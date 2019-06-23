#ifndef CALLBACK_H_
#define CALLBACK_H_

#include <functional>
#include <memory>

#include "InetAddress.h"
#include "TcpConnection.h"

namespace muduo
{
using TimerCallback = std::function<void()>;
using WakeupCallback = std::function<void()>;
using NewConnectionCallback = std::function<void(int sockfd, const InetAddress& addr)>;
using ConnectionCallback = std::function<void(std::shared_ptr<TcpConnection>)>;
using MessageCallback = std::function<void(std::shared_ptr<TcpConnection>, char* data, size_t len)>;
}

#endif