#ifndef CALLBACK_H_
#define CALLBACK_H_

#include <functional>

namespace muduo
{
using TimerCallback = std::function<void()>;
using WakeupCallback = std::function<void()>;
}

#endif