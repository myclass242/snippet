#include "TimerQueue.h"

#include <string.h>

#include <algorithm>
#include <functional>

#include <sys/timerfd.h>

#include "Eventloop.h"
#include "Timer.h"
#include "base/Logging.h"

using namespace muduo;

namespace
{
int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC | TFD_NONBLOCK);
    if (timerfd < 0)
    {
        LOG_SYSFATAL << "Failed in timerfd_create";
    }

    return timerfd;
}

void readTimerfd(int timerfd, Timestamp now)
{
    uint64_t howmany;
    auto ret = ::read(timerfd, &howmany, sizeof(howmany));
    LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
    if (ret != sizeof(howmany))
    {
        LOG_ERROR << "TImerQueue::handleRead() reads " << ret << " bytes instead of 8, error = " << strerror(errno);
    }
}

struct timespec howMuchTimeFromNow(Timestamp when)
{
    int64_t microseconds = when.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();
    if (microseconds < 100)
    {
        microseconds = 100;
    }

    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(microseconds / Timestamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>(1000 * (microseconds % Timestamp::kMicroSecondsPerSecond));
    
    return ts;
}

void resetTimerfd(int timerFd, Timestamp expiration)
{
    struct itimerspec oldTs, newTs;
    ::memset(&oldTs, 0, sizeof(oldTs));
    ::memset(&newTs, 0, sizeof(newTs));

    newTs.it_value = howMuchTimeFromNow(expiration);

    auto ret = ::timerfd_settime(timerFd, 0, &newTs, &oldTs);
    if (ret == -1)
    {
        LOG_SYSERR << "timerfd_settime";
    }
}
}

TimerQueue::TimerQueue(Eventloop* loop)
    : loop_(loop), timerFd_(createTimerfd())
{
    timerFdChannel_ = std::make_shared<Channel>(loop_, timerFd_);
    timerFdChannel_->setReadCallback(std::bind(&TimerQueue::handleRead, this));
    timerFdChannel_->enableReading();
}

TimerQueue::~TimerQueue()
{
    ::close(timerFd_);
    // do not remove channel, since wo are in Eventloop::dtor()
    for (auto& entry : timerList_)
    {
        delete entry.second;
    }
}

TimerId TimerQueue::addTimer(TimerCallback&& cb, Timestamp when, double interval)
{
    Timer* timer = new Timer(std::move(cb), when, interval);
    loop_->runInLoop(std::bind(&TimerQueue::addTimerInloop, this, timer));

    return TimerId{timer};
}

void TimerQueue::addTimerInloop(Timer* timer)
{
    loop_->assertInLoopThread();
    
    bool early = insert(timer);
    if (early)
    {
        resetTimerfd(timerFd_, timer->expiration());
    }
}

std::vector<TimerQueue::Entry> TimerQueue::getExpiration(Timestamp now)
{
    std::vector<Entry> expirations;
    Entry tmp{now, nullptr};

    auto first = std::lower_bound(timerList_.begin(), timerList_.end(), tmp);
    assert(first == timerList_.end() || now < first->first);
    std::copy(timerList_.begin(), first, std::back_inserter(expirations));
    timerList_.erase(timerList_.begin(), first);

    return expirations;
}

bool TimerQueue::insert(Timer* timer)
{
    bool early = false;
    Timestamp expiration = timer->expiration();
    auto timerListBegin = timerList_.begin();
    if (timerListBegin == timerList_.end() || expiration < timerListBegin->first)
    {
        early = true;
    }
    auto result = timerList_.insert({expiration, timer});
    assert(result.second);

    return early;
}

void TimerQueue::reset(const std::vector<Entry>& expirations, Timestamp now)
{
    for (auto ite = expirations.begin(); ite != expirations.end(); ++ite)
    {
        if (ite->second->repeat())
        {
            ite->second->restart(now);
            insert(ite->second);
        }
        else
        {
            delete ite->second;
        }
    }

    if (!timerList_.empty())
    {
        Timestamp nextExpiration = timerList_.begin()->first;
        resetTimerfd(timerFd_, nextExpiration);
    }
}

void TimerQueue::handleRead()
{
    loop_->assertInLoopThread();

    Timestamp now = Timestamp::now();
    readTimerfd(timerFd_, now);

    auto expirations = getExpiration(now);
    for (const auto& expiration : expirations)
    {
        expiration.second->run();
    }

    reset(expirations, now);
}