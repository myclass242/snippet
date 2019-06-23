#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#include "Eventloop.h"
#include "Acceptor.h"
#include "base/noncopyable.h"
#include "TcpConnection.h"
#include "Callback.h"
#include "InetAddress.h"

#include <string>
#include <unordered_map>
#include <memory>

namespace muduo
{
class TcpServer
{
public:
    TcpServer(Eventloop* loop, const InetAddress& addr, const std::string& name);
    TcpServer(Eventloop* loop, const InetAddress& addr);
    ~TcpServer()
    {}

    void start();

    void setConnectionCallback(ConnectionCallback&& cb)
    {
        connectionCallback_ = std::move(cb);
    }
    void setMessageCallback(MessageCallback&& cb)
    {
        messageCallback_ = std::move(cb);
    }

private:
    void newConnection(int connFd, const InetAddress& peerAddr);

    using ConnectionMap = std::unordered_map<std::string, std::shared_ptr<TcpConnection>>;

    Eventloop* loop_;
    const std::string name_;
    std::shared_ptr<Acceptor> acceptor_;
    ConnectionMap connMap_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    uint64_t nextConnection_;
    bool start_;
};
}

#endif