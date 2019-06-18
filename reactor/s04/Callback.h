#ifndef CALLBACK_H_
#define CALLBACK_H_

#include <functional>

#include "InetAddress.h"

namespace muduo
{
using TimerCallback = std::function<void()>;
using WakeupCallback = std::function<void()>;
using NewConnectionCallback = std::function<void(int sockfd, const InetAddress& addr)>;
}

#endif