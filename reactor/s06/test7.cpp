#include <stdio.h>

#include <unistd.h>

#include "Eventloop.h"
#include "Acceptor.h"
#include "SockOps.h"

using namespace muduo;

void newConnection1(int connfd, const InetAddress& peer)
{
    printf("newConnection: accept a new connection from %s\n", peer.hostPort().c_str());
    ::write(connfd, "How are you?\n", 13);
    sockets::close(connfd);
}

void newConnection2(int connfd, const InetAddress& peer)
{
    printf("newConnection: accept a new connection from %s\n", peer.hostPort().c_str());
    ::write(connfd, "Hello World!\n", 13);
    sockets::close(connfd);
}

int main(void)
{
    printf("main: pid = %d\n", getpid());

    Eventloop loop;

    InetAddress addr1{9981};
    Acceptor acceptor1(&loop, addr1);
    acceptor1.setNewConnectionCallback(newConnection1);
    acceptor1.listen();

    InetAddress addr2(9982);
    Acceptor acceptor2(&loop, addr2);
    acceptor2.setNewConnectionCallback(newConnection2);
    acceptor2.listen();
    
    loop.loop();
}