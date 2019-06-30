#include "TcpConnection.h"

#include <unistd.h>

#include "base/Logging.h"
#include "SockOps.h"


using namespace muduo;

TcpConnection::TcpConnection(Eventloop* loop, const std::string& name, int connFd,
                            const InetAddress& localAddr, const InetAddress& peerAddr)
    : loop_(loop), connectedFd_(connFd), name_(name), channel_(std::make_shared<Channel>(loop, connFd)),
        localAddr_(localAddr), peerAddr_(peerAddr), state_(Connecting)
{
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
}

TcpConnection::~TcpConnection()
{
    LOG_DEBUG << "TcpConnection::~TcpConnection " << name_;
}

void TcpConnection::connectEstablished()
{
    loop_->assertInLoopThread();
    assert(state_ == Connecting);
    setState(Connected);
    connectionCallback_(shared_from_this());
    channel_->enableReading();
    channel_->enableWriting();
}

void TcpConnection::connectDestoryed()
{
    loop_->assertInLoopThread();
    assert(state_ == Connected);
    setState(Disconnected);
    channel_->disableAll();
    auto self = shared_from_this();
    connectionCallback_(self);
    loop_->removeChannel(channel_);
}

void TcpConnection::handleRead()
{
    char buf[65536];
    auto n = ::read(channel_->fd(), buf, sizeof(buf));
    if (n > 0)
    {
        messageCallback_(shared_from_this(), buf, n);
    }
    else if (n == 0)
    {
        handleClose();
    }
    else
    {
        handleError();
    }
    
}

void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();
    LOG_TRACE << "TcpConnection::handleClose state " << state_;
    assert(state_ == Connected);
    channel_->disableAll();
    // closeCallback_会删除TcpServer保存的TcpConnection智能指针，在调用closeCallback_
    // 后次TcpConnection会析构，导致closeCallback_（self）后的函数不会调用，因此提前保存TcpConnection
    // 的智能指针，使closeCallback_后不会析构TcpConnection
    auto self = shared_from_this();
    closeCallback_(self);

    // 此函数在channel的closeCallback回调里面，因此不能直接删除channel,不然channel
    // 析构会core dump，原因是channel的普通函数调用会删除自己。是否可以考虑channel为shared_ptr而不是裸指针
    setState(Disconnected);
    connectionCallback_(self);
    // loop_->queueInloop(std::bind(&Eventloop::removeChannel, loop_, channel_));
    loop_->removeChannel(channel_);
}

void TcpConnection::handleError()
{
    int err = sockets::getSocketError(connectedFd_);
    LOG_TRACE << "TcpConnection::handleError [" << name_ << "] SO_ERROR = " <<  err << strerror_tl(err);
}

void TcpConnection::handleWrite()
{

}