#include "Socket.h"

#include <sys/types.h>
#include <sys/socket.h>

#include "InetAddress.h"
#include "SockOps.h"

using namespace muduo;

Socket::~Socket()
{
    sockets::close(sockfd_);
}

void Socket::bind(const InetAddress& addr) const
{
    sockets::bindOrDie(sockfd_, addr.get());
}

void Socket::listen(void) const
{
    sockets::listenOrDie(sockfd_);
}

int Socket::accept(InetAddress* peer)
{
    return sockets::accept(sockfd_, &(peer->getRef()));
}

void Socket::setReuseAddr(bool on)
{
  int optval = on ? 1 : 0;
  ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR,
               &optval, sizeof optval);
  // FIXME CHECK
}