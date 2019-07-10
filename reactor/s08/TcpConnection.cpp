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
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
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
    // channel_->enableWriting();
}

void TcpConnection::connectDestoryed()
{
    loop_->assertInLoopThread();
    assert(state_ == Disconnecting);
    setState(Disconnected);
    channel_->disableAll();
    auto self = shared_from_this();
    connectionCallback_(self);
    loop_->removeChannel(channel_);
}

void TcpConnection::handleRead(Timestamp receiveTime)
{
    int savedErrno = 0;
    auto n = inputBuffer_.readFd(connectedFd_, &savedErrno);
    if (n > 0)
    {
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    }
    else if (n == 0)
    {
        handleClose();
    }
    else
    {
        errno = savedErrno;
        LOG_SYSERR << "Tcpconnection::handleRead";
        handleError();
    }
    
}

void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();
    LOG_TRACE << "TcpConnection::handleClose state " << state_;
    assert(state_ == Disconnecting);
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

void TcpConnection::send(const std::string& message)
{
    if (state_ == Connected)
    {
        if (loop_->isInLoopThread())
        {
            sendInloop(message);
        }
        else
        {
            loop_->runInLoop(std::bind(&TcpConnection::sendInloop, this, message));
        }
    }
}

void TcpConnection::sendInloop(const std::string& message)
{
    loop_->assertInLoopThread();

    ssize_t nwrite = 0;
    if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
    {
        nwrite = ::write(connectedFd_, message.data(), message.size());
        if (nwrite >= 0)
        {
            if (implicit_cast<size_t>(nwrite) < message.size())
            {
                LOG_TRACE << "I am going to write more data";
            }
        } else {
            nwrite = 0;
            if (errno != EWOULDBLOCK)
            {
                LOG_SYSERR << "TcpConnection::sendInloop";
            }
        }
    }

    assert(nwrite >= 0);
    if (implicit_cast<size_t>(nwrite) < message.size())
    {
        outputBuffer_.append(message.data() + nwrite, message.size() - nwrite);
        if (!channel_->isWriting())
        {
            channel_->enableWriting();
        }
    }
}

void TcpConnection::handleWrite()
{
    loop_->assertInLoopThread();
    if (channel_->isWriting() && outputBuffer_.readableBytes() > 0)
    {
        ssize_t n = write(connectedFd_, outputBuffer_.peek(), outputBuffer_.readableBytes());
        if (n > 0)
        {
            outputBuffer_.retrieve(n);
            if (outputBuffer_.readableBytes() == 0)
            {
                channel_->disableWriting();
                if (state_ == Disconnecting)
                {
                    shutdownInloop();
                }
            }
            else
            {
                LOG_TRACE << "I am going to write more data";
            }
        }
        else
        {
            LOG_SYSERR << "TcpConnection::handleWrite";
        }
    }
    else
    {
        LOG_TRACE << "Connecting is down, no more writing";
    }
}

void TcpConnection::shutdownInloop()
{
    loop_->assertInLoopThread();
    if (!channel_->isWriting())
    {
        ::shutdown(connectedFd_, SHUT_WR);
    }
}

void TcpConnection::shutdown()
{
    if (state_ == Connected)
    {
        setState(Disconnecting);
        loop_->runInLoop(std::bind(&TcpConnection::shutdownInloop, this));
    }
}