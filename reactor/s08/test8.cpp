#include "Eventloop.h"
#include "TcpServer.h"
#include "InetAddress.h"
#include "TcpConnection.h"
#include "Buffer.h"
#include "base/Timestamp.h"

#include <memory>
#include <iostream>

#include <unistd.h>

using namespace muduo;

void onConnection(const std::shared_ptr<TcpConnection>& conn)
{
    if (conn->connected())
    {
        std::cout << "onConnection(): new Connection " << conn->name()
                  << " from " << conn->peerAddr().hostPort() << " to "
                  << conn->localAddr().hostPort() << '\n';
    }
    else
    {
        std::cout << "onConnection(): Connection [" << conn->name() << "] is down" << '\n'; 
    }
}

void onMessage(const std::shared_ptr<TcpConnection>& conn, Buffer* buffer, Timestamp receiveTime)
{
    std::cout << "onMessage() :: received " << buffer->readableBytes() << " bytes from " << conn->name();
    std::cout << "\nMessage is " << buffer->retrieveAsString();
}

int main(void)
{
    std::cout << "main(): pid = " << getpid() << '\n';

    Eventloop loop;
    InetAddress addr(9981);

    TcpServer server(&loop, addr);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();

    loop.loop();

    return 0;
}