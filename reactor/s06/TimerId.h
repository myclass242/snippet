#ifndef TIMER_ID_H_
#define TIMER_ID_H_

#include "Timer.h"
#include "base/copyable.h"


namespace muduo
{
class TimerId : public copyable
{
public:
    explicit TimerId(Timer* timer)
                : value_(timer)
    {}
private:
    Timer* value_;
};
}

#endif