#include "Timer.h"

#include <utility>


using namespace muduo;

Timer::Timer(TimerCallback&& cb, TimeStamp when, double interval)
    : callback_(std::move(cb)), expiration_(when),
      interval_(interval), repeat_(interval > 0)
{
}

void Timer::restart(Timestamp now)
{
    if (repeat_)
    {
        expiration_ = addTime(now, interval_);
    }
    else
    {
        expiration_ = Timestamp::invalid();
    }
    
}