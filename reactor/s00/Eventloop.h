//
// Created by zy on 5/26/19.
//

#ifndef S00_EVENTLOOP_H
#define S00_EVENTLOOP_H


#include <atomic>

#include <sys/types.h>

#include "base/noncopyable.h"
#include "base/CurrentThread.h"


namespace muduo {

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

    static Eventloop* getEventloopOfCurrentThread();
private:
    void abortNotInLoopThread();
private:
    std::atomic_bool looping_;
    const pid_t threadId_;

};

}

#endif //S00_EVENTLOOP_H
