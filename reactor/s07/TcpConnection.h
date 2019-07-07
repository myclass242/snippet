#ifndef TCP_CONNECTION_H_
#define TCP_CONNECTION_H_

#include <string>
#include <memory>

#include "base/noncopyable.h"
#include "Buffer.h"
#include "InetAddress.h"
#include "Eventloop.h"
#include "Callback.h"
#include "Channel.h"
#include "base/Timestamp.h"


namespace muduo
{
class TcpConnection : public std::enable_shared_from_this<TcpConnection>, public noncopyable
{
    enum State {Connecting, Connected, Disconnected};
    using CloseCallback = std::function<void(std::shared_ptr<TcpConnection>)>;
public:
    TcpConnection(Eventloop* loop, const std::string& name, int connFd, const InetAddress& localAddr, const InetAddress& peerAddr);
    ~TcpConnection();

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
    void connectDestoryed();

    void setConnectionCallback(const ConnectionCallback& cb)
    {
        connectionCallback_ = cb;
    }
    void setMessageCallback(const MessageCallback& cb)
    {
        messageCallback_ = cb;
    }
    void setCloseCallback(const CloseCallback& cb)
    {
        closeCallback_ = cb;
    }
private:
    void setState(State state)
    {
        state_ = state;
    }
    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleError();
    void handleClose();

    Eventloop* loop_;
    int connectedFd_;
    const std::string name_;
    ChannelPtr channel_;
    InetAddress localAddr_;
    InetAddress peerAddr_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    CloseCallback closeCallback_;
    State state_;
    Buffer inputBuffer_;
};
}

#endif