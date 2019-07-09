#ifndef TIMER_H_
#define TIMER_H_

#include "Callback.h"
#include "base/noncopyable.h"
#include "base/Timestamp.h"


namespace muduo
{
class Timer : public noncopyable
{
public:
    Timer(TimerCallback&& cb, Timestamp when, double interval);

    void run() const
    {
        callback_();
    }

    Timestamp expiration() const noexcept
    {
        return expiration_;
    }
    bool repeat() const noexcept
    {
        return repeat_;
    }

    void restart(Timestamp now);
private:
    const TimerCallback callback_;
    Timestamp expiration_;
    double interval_;
    bool repeat_;
};
}

#endif