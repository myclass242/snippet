#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include <string>
#include <memory>

#include "base/noncopyable.h"
#include "InetAddress.h"
#include "Eventloop.h"
#include "Callback.h"
#include "Channel.h"


namespace muduo
{
class TcpConnection : public std::enable_shared_from_this<TcpConnection>, public noncopyable
{
    enum State {Connecting, Connected};
public:
    TcpConnection(Eventloop* loop, const std::string& name, int connFd, const InetAddress& localAddr, const InetAddress& peerAddr);
    ~TcpConnection()
    {}

    std::string name(void) const
    {
        return name_;
    }

    bool connected(void) const
    {
        return Connected == state_;
    }

    const InetAddress& localAddr(void) const
    {
        return localAddr_;
    }

    const InetAddress& peerAddr(void) const
    {
        return peerAddr_;
    }

    void connectEstablished();

    void setConnectionCallback(const ConnectionCallback& cb)
    {
        connectionCallback_ = cb;
    }
    void setMessageCallback(const MessageCallback& cb)
    {
        messageCallback_ = cb;
    }
private:
    void setState(State state)
    {
        state_ = state;
    }
    void handleRead();

    Eventloop* loop_;
    int connectedFd_;
    const std::string name_;
    std::shared_ptr<Channel> channel_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    State state_;
};
}

#endif