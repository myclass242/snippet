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


namespace muduo {
class Poller;
class Channel;

class Eventloop : public noncopyable
{
public:
    Eventloop();
    ~Eventloop();

    void loop();
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

    void quit() noexcept
    {
        quit_ = true;
    }

    void updateChannel(Channel* channel);

    static Eventloop* getEventloopOfCurrentThread();
private:
    void abortNotInLoopThread();
private:
    using ChannelLIst = std::vector<Channel*>;

    std::atomic_bool looping_;
    const pid_t threadId_;
    std::atomic_bool quit_;
    std::unique_ptr<Poller> poller_;
    ChannelLIst activeChannels_;

};

}

#endif //S00_EVENTLOOP_H
