#ifndef CALLBACK_H_
#define CALLBACK_H_

#include <functional>
#include <memory>

#include "InetAddress.h"

namespace muduo
{
class TcpConnection;
using TcpPtr = std::shared_ptr<TcpConnection>;

using TimerCallback = std::function<void()>;
using NewConnectionCallback = std::function<void(int sockfd, const InetAddress& addr)>;
using ConnectionCallback = std::function<void(const TcpPtr&)>;
using MessageCallback = std::function<void(const TcpPtr&, char* data, size_t len)>;
}

#endif