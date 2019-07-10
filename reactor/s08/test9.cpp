#include "Eventloop.h"
#include "TcpServer.h"
#include "TcpConnection.h"
#include "Buffer.h"
#include "InetAddress.h"
#include "base/Timestamp.h"

#include <memory>
#include <iostream>

using namespace muduo;

void on_connection(const std::shared_ptr<TcpConnection>& conn)
{
    if (conn->connected())
    {
        std::cout << "on_connection: new connection name: " << conn->name()
                  << " from " << conn->peerAddr().hostPort() << " to "
                  << conn->localAddr().hostPort() << std::endl;
    }
    else
    {
        std::cout << "on_connection: connection " << conn->name() << " is down" << std::endl;
    }
}

void on_message(const std::shared_ptr<TcpConnection>& conn, Buffer* buffer, Timestamp receiveTime)
{
    std::cout << "onMessage() :: received " << buffer->readableBytes() << " bytes from " << conn->name();
    std::string message = buffer->retrieveAsString();
    std::cout << "\nMessage is " << message;
    conn->send(message);
}

int main(void)
{
    Eventloop loop;
    InetAddress addr(9981);
    TcpServer tcpServer(&loop, addr);

    tcpServer.setConnectionCallback(on_connection);
    tcpServer.setMessageCallback(on_message);
    tcpServer.start();

    loop.loop();

    return 0;
}