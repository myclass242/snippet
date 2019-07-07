//
// Created by zy on 5/26/19.
//

#include "Eventloop.h"

#include <poll.h>
#include <sys/eventfd.h>

#include "base/Logging.h"
#include "Poller.h"
#include "Channel.h"
#include "TimerQueue.h"

using namespace muduo;


thread_local Eventloop* t_loopInThisThread = nullptr;
namespace
{
const int kPollTimeMs = 500;

int createEventFd()
{
    int eventFd = ::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
    if (-1 == eventFd)
    {
        LOG_SYSERR << "Failed in eventfd";
        abort();
    }

    return eventFd;
}
}

Eventloop::Eventloop()
    : looping_(false), threadId_(CurrentThread::tid()),
      quit_(false), wakeupFd_(createEventFd()),
      callingPendingTasks_(false)
{
    poller_.reset(new Poller(this));
    timerQueue_.reset(new TimerQueue(this));
    wakeupChannel_ = std::make_shared<Channel>(this, wakeupFd_);
    wakeupChannel_->setReadCallback(std::bind(&Eventloop::handleWakeup, this));
    wakeupChannel_->enableReading();
    LOG_TRACE << "Eventloop created " << this << " in thread " << threadId_;

    if (t_loopInThisThread)
    {
        LOG_FATAL << "Another Eventloop " << t_loopInThisThread << " exists in this thread " << threadId_;
    }
    else
    {
        t_loopInThisThread = this;
    }
}

Eventloop::~Eventloop()
{
    assert(!looping_);
    t_loopInThisThread = nullptr;
}

void Eventloop::abortNotInLoopThread()
{
    LOG_ERROR << "Not in loop thread";
    abort();
}

Eventloop* Eventloop::getEventloopOfCurrentThread()
{
    return t_loopInThisThread;
}

void Eventloop::loop()
{
    assert(!looping_);
    assertInLoopThread();

    looping_ = true;
    quit_ = false;

    while (!quit_) {
        activeChannels_.clear();
        pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
        for (auto& channel : activeChannels_) {
            channel->handleEvent(pollReturnTime_);
        }
        doPendingTasks();
    }

    LOG_TRACE << "Eventloop " << this << " stop looping";
    looping_ = false;
}

void Eventloop::quit()
{
    quit_ = true;
    if (!isInLoopThread())
    {
        wakeup();
    }
}

TimerId Eventloop::runAt(TimerCallback&& callback, Timestamp when)
{
    return timerQueue_->addTimer(std::move(callback), when, 0.0);
}

TimerId Eventloop::runAfter(TimerCallback&& callback, double delay)
{
    Timestamp when = addTime(Timestamp::now(), delay);
    return runAt(std::move(callback), when);
}

TimerId Eventloop::runEvery(TimerCallback&& callback, double interval)
{
    Timestamp when = addTime(Timestamp::now(), interval);
    return timerQueue_->addTimer(std::move(callback), when, interval);
}

void Eventloop::updateChannel(muduo::ChannelPtr channel)
{
    assert(channel->ownloop() == this);
    assertInLoopThread();

    poller_->updateChannel(channel);
}

void Eventloop::removeChannel(ChannelPtr channel)
{
    assert(channel->ownloop() == this);
    assertInLoopThread();

    poller_->removeChannel(channel);
}

void Eventloop::doPendingTasks()
{
    decltype(pendingTasks_) tasks;
    callingPendingTasks_ = true;
    {
        MutexLockGuard lock(mutex_);
        tasks.swap(pendingTasks_);
    }
    for (auto& task : tasks)
    {
        task();
    }
    callingPendingTasks_ = false;
}

void Eventloop::handleWakeup()
{
    uint64_t one = 1;
    auto n = ::read(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one))
    {
        LOG_ERROR << "Eventloop::handleWakeup read " << n << " bytes instead of " << sizeof(one);
    }
}

void Eventloop::wakeup()
{
    uint64_t one = 1;
    auto n = ::write(wakeupFd_, &one, sizeof(one));
    if (n != sizeof(one))
    {
        LOG_ERROR << "Eventloop::wakeup writes " << n << " bytes instead of " << sizeof(one);
    }
}

void Eventloop::addPendingTask(WakeupCallback&& wakeupCallback)
{
    {
        MutexLockGuard lock(mutex_);
        pendingTasks_.push_back(std::move(wakeupCallback));
    }

    if (!isInLoopThread() || callingPendingTasks_)
    {
        wakeup();
    }
}

void Eventloop::runInLoop(WakeupCallback&& wakeupCallback)
{
    if (isInLoopThread())
    {
        wakeupCallback();
    }
    else
    {
        addPendingTask(std::move(wakeupCallback));
    }
}