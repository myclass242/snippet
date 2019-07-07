#include "EventLoopThread.h"

#include <functional>

using namespace muduo;

EventLoopThread::EventLoopThread()
    : loop_(nullptr),
      thread_(std::bind(&EventLoopThread::threadFunc, this)),
      mutex_(),
      cond_(mutex_)
{}

EventLoopThread::~EventLoopThread()
{
    loop_->quit();
    thread_.join();
}

Eventloop* EventLoopThread::startLoop()
{
    assert(!thread_.started());
    thread_.start();

    {
        MutexLockGuard lock(mutex_);
        while (nullptr == loop_)
        {
            cond_.wait();
        }
    }

    return loop_;
}

void EventLoopThread::threadFunc()
{
    Eventloop loop;
    {
        MutexLockGuard lock(mutex_);
        loop_ = &loop;
        cond_.notify();
    }
    loop_->loop();
}