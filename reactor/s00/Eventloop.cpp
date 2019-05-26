//
// Created by zy on 5/26/19.
//

#include "Eventloop.h"

#include <poll.h>

#include "base/Logging.h"

using namespace muduo;


thread_local Eventloop* t_loopInThisThread = nullptr;

Eventloop::Eventloop()
    : looping_(false), threadId_(CurrentThread::tid())
{
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

    ::poll(NULL, 0, 5*1000);

    LOG_TRACE << "Eventloop " << this << " stop looping";
    looping_ = false;
}