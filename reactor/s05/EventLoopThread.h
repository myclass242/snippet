#ifndef EVENT_LOOP_THREAD_H_
#define EVENT_LOOP_THREAD_H_

#include "Eventloop.h"
#include "base/Mutex.h"
#include "base/Condition.h"
#include "base/noncopyable.h"
#include "base/Thread.h"

namespace muduo
{
class EventLoopThread : public noncopyable
{
public:
    EventLoopThread();
    ~EventLoopThread();
    
    Eventloop* startLoop(void);
private:
    void threadFunc();

    Eventloop* loop_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
};
}

#endif