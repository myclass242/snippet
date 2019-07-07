//
// Created by zy on 5/26/19.
//

#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_


#include <atomic>
#include <memory>
#include <vector>

#include <sys/types.h>

#include "base/noncopyable.h"
#include "base/CurrentThread.h"
#include "base/Mutex.h"
#include "TimerId.h"
#include "Channel.h"


namespace muduo {
class Poller;
class TimerQueue;

class Eventloop : public noncopyable
{
    using WakeupCallback = std::function<void()>;
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

    void queueInloop(WakeupCallback&& wakeupCallback)
    {
        addPendingTask(std::move(wakeupCallback));
    }

    void quit();

    void updateChannel(ChannelPtr channel);
    void removeChannel(ChannelPtr channel);

    static Eventloop* getEventloopOfCurrentThread();
private:
    void abortNotInLoopThread();
    void handleWakeup();
    void doPendingTasks();
    void wakeup();
    void addPendingTask(WakeupCallback&& wakeupCallback);
private:
    using ChannelLIst = std::vector<ChannelPtr>;

    std::atomic_bool looping_;
    const pid_t threadId_;
    std::atomic_bool quit_;
    std::unique_ptr<Poller> poller_;
    ChannelLIst activeChannels_;
    std::unique_ptr<TimerQueue> timerQueue_;
    int wakeupFd_;
    ChannelPtr wakeupChannel_;
    std::vector<WakeupCallback> pendingTasks_;
    MutexLock mutex_;
    std::atomic_bool callingPendingTasks_;
    Timestamp pollReturnTime_;
};
}

#endif //EVENTLOOP_H_
