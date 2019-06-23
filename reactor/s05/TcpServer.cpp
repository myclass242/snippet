#include "TcpServer.h"

#include <sstream>

#include "base/Logging.h"
#include "SockOps.h"

using namespace muduo;

TcpServer::TcpServer(Eventloop* loop, const InetAddress& addr, const std::string& name)
    : loop_(loop), name_(name), nextConnection_(0), start_(false)
{
    acceptor_ = std::make_shared<Acceptor>(loop_, addr);
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

TcpServer::TcpServer(Eventloop* loop, const InetAddress& addr)
    : loop_(loop), name_(addr.hostPort()), nextConnection_(0), start_(false)
{
    acceptor_ = std::make_shared<Acceptor>(loop_, addr);
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2));
}

void TcpServer::start()
{
    start_ = true;
    if (!acceptor_->isListen())
    {
        loop_->runInLoop(std::bind(&Acceptor::listen, acceptor_.get()));
    }
}

void TcpServer::newConnection(int connFd, const InetAddress& peerAddr)
{
    loop_->assertInLoopThread();

    std::stringstream isstream;
    std::string connectionName;
    ++nextConnection_;
    isstream << name_ << "#" << nextConnection_;
    isstream >> connectionName;

    LOG_INFO << "TcpServer::newConnection [" << name_ << "] - new connection ["
             << connectionName << "] from " << peerAddr.hostPort();

    InetAddress localAddr = sockets::getLocalAddr(connFd);
    std::shared_ptr<TcpConnection> conn = std::make_shared<TcpConnection>(loop_, connectionName, connFd, localAddr, peerAddr);
    connMap_[connectionName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->connectEstablished();
}