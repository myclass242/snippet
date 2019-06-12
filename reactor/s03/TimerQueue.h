#ifndef TIMER_QUEUE_H_
#define TIMER_QUEUE_H_

// #include "Timer.h"
#include "TimerId.h"
#include "base/noncopyable.h"
#include "Channel.h"
#include "base/Timestamp.h"
// #include "Eventloop.h"

#include <set>
#include <vector>

namespace muduo
{
class Eventloop;
class Timer;
class TimerQueue : public noncopyable
{
public:
    explicit TimerQueue(Eventloop* loop);
    ~TimerQueue();

    /*
        Schedule the callback at given time
        repeat if interval > 0
        Must be thread safe, Usually be called from other thread
     */
    TimerId addTimer(TimerCallback&& cb, Timestamp when, double interval);

private:
    using Entry = std::pair<Timestamp, Timer*>;
    using TimerList = std::set<Entry>;

    void handleRead();
    std::vector<Entry> getExpiration(Timestamp now);
    void reset(const std::vector<Entry>& expirations, Timestamp now);

    bool insert(Timer* timer);
    void addTimerInloop(Timer* timer);

    Eventloop* loop_;
    const int timerFd_;
    Channel timerFdChannel_;
    // timerList sorted by expiration
    TimerList timerList_;
};
}

#endif