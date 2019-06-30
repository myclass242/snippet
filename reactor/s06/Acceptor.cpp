#include "Acceptor.h"

#include "SockOps.h"

using namespace muduo;

Acceptor::Acceptor(Eventloop* loop, const InetAddress& addr)
    : loop_(loop),
      acceptSocket_(sockets::createNonblockOrDie()),
      listening_(false)
{
    acceptChannel_ = std::make_shared<Channel>(loop_, acceptSocket_.fd());
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.bind(addr);
    acceptChannel_->setReadCallback(std::bind(&Acceptor::handleRead, this));
}

void Acceptor::listen()
{
    loop_->assertInLoopThread();
    acceptSocket_.listen();
    listening_ = true;
    acceptChannel_->enableReading();
}

void Acceptor::handleRead()
{
    loop_->assertInLoopThread();

    int connfd = -1;
    while (true)
    {
        InetAddress peer{0};
        int connfd = acceptSocket_.accept(&peer);
        if (connfd >= 0)
        {
            if (callback_)
            {
                callback_(connfd, peer);
            }
            else
            {
                sockets::close(connfd);
            }
        }
        else
        {
            break;
        }
    }
}