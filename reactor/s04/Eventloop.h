//
// Created by zy on 5/26/19.
//

#ifndef S00_EVENTLOOP_H
#define S00_EVENTLOOP_H


#include <atomic>
#include <memory>
#include <vector>

#include <sys/types.h>

#include "base/noncopyable.h"
#include "base/CurrentThread.h"
#include "base/Mutex.h"
#include "Callback.h"
#include "TimerId.h"


namespace muduo {
class Poller;
class Channel;
class TimerQueue;

class Eventloop : public noncopyable
{
public:
    Eventloop();
    ~Eventloop();

    void loop();

    void runInLoop(WakeupCallback&& wakeupCallback);

    Timestamp pollReturnTimer() const noexcept
    {
        return pollReturnTime_;
    }

    TimerId runAt(TimerCallback&& callBack, Timestamp when);
    TimerId runAfter(TimerCallback&& callBack, double delay);
    TimerId runEvery(TimerCallback&& callBack, double interval);


    void assertInLoopThread()
    {
        if (!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }

    bool isInLoopThread() const
    {
        return threadId_ == CurrentThread::tid();
    }

    void quit();

    void updateChannel(Channel* channel);

    static Eventloop* getEventloopOfCurrentThread();
private:
    void abortNotInLoopThread();
    void handleWakeup();
    void doPendingTasks();
    void wakeup();
    void addPendingTask(WakeupCallback&& wakeupCallback);
private:
    using ChannelLIst = std::vector<Channel*>;

    std::atomic_bool looping_;
    const pid_t threadId_;
    std::atomic_bool quit_;
    std::unique_ptr<Poller> poller_;
    ChannelLIst activeChannels_;
    std::unique_ptr<TimerQueue> timerQueue_;
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    std::vector<WakeupCallback> pendingTasks_;
    MutexLock mutex_;
    std::atomic_bool callingPendingTasks_;
    Timestamp pollReturnTime_;
};

}

#endif //S00_EVENTLOOP_H
