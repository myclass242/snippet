#include "TcpConnection.h"

#include <unistd.h>

using namespace muduo;

TcpConnection::TcpConnection(Eventloop* loop, const std::string& name, int connFd,
                            const InetAddress& localAddr, const InetAddress& peerAddr)
    : loop_(loop), connectedFd_(connFd), name_(name), channel_(std::make_shared<Channel>(loop, connFd)),
        localAddr_(localAddr), peerAddr_(peerAddr), state_(Connecting)
{
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
}

void TcpConnection::connectEstablished()
{
    loop_->assertInLoopThread();
    assert(state_ == Connecting);
    setState(Connected);
    connectionCallback_(shared_from_this());
    channel_->enableReading();
}

void TcpConnection::handleRead()
{
    char buf[65536];
    auto n = ::read(channel_->fd(), buf, sizeof(buf));
    messageCallback_(shared_from_this(), buf, n);
}