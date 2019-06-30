#include "SockOps.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include "base/Logging.h"

namespace muduo
{
namespace sockets
{
using SA = struct sockaddr;

inline const SA* sockaddr_cast(const sockaddr_in* addr)
{
    return reinterpret_cast<const SA*>(addr);
}

inline SA* sockaddr_cast(sockaddr_in* addr)
{
    return reinterpret_cast<SA*>(addr);
}

int createNonblockOrDie()
{
    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_SYSFATAL << "muduo::createNonblockOrDie";
    }
    return sockfd;
}

void bindOrDie(int sockfd, const struct sockaddr_in& addr)
{
    if (bind(sockfd, sockaddr_cast(&addr), sizeof(addr)) < 0)
    {
        LOG_SYSFATAL << "muduo::bindOrDie";
    }
}

void listenOrDie(int sockfd)
{
    if (listen(sockfd, SOMAXCONN) < 0)
    {
        LOG_SYSFATAL << "muduo::listenOrDie";
    }
}

int accept(int sockfd, struct sockaddr_in* addr)
{
    socklen_t len = sizeof(sockaddr_in);
    int connfd = accept4(sockfd, sockaddr_cast(addr), &len, SOCK_CLOEXEC | SOCK_NONBLOCK);
    if (connfd < 0)
    {
        int saveErrno = errno;
        LOG_SYSERR << "muduo::accept";
        
        switch (saveErrno)
        {
            case EAGAIN:
            // case EWOULDBLOCK:    duplicate case value
            case EINTR:
            case EMFILE:
            case EPERM:
            case ECONNABORTED:
            case EPROTO:
            errno = saveErrno;
            break;

            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
            // unexpected errors
            LOG_FATAL << "unexpected error of ::accept " << saveErrno;
            break;

            default:
            LOG_FATAL << "unknown error of ::accept " << saveErrno;
            break;
        }
    }

    return connfd;
}

void close(int sockfd)
{
    if (::close(sockfd) < 0)
    {
        LOG_SYSERR << "muduo::close";
    }
}

void toHostPort(char* buf, size_t size,
                         const struct sockaddr_in& addr)
{
  char host[INET_ADDRSTRLEN] = "INVALID";
  ::inet_ntop(AF_INET, &addr.sin_addr, host, sizeof host);
  uint16_t port = sockets::networkToHost16(addr.sin_port);
  snprintf(buf, size, "%s:%u", host, port);
}

void fromHostPort(const char* ip, uint16_t port,
                           struct sockaddr_in* addr)
{
  addr->sin_family = AF_INET;
  addr->sin_port = hostToNetwork16(port);
  if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
  {
    LOG_SYSERR << "sockets::fromHostPort";
  }
}

InetAddress getLocalAddr(int sockFd)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    socklen_t len = 0;
    if (-1 == getsockname(sockFd, sockaddr_cast(&addr), &len))
    {
        LOG_SYSERR << "sockets::getLocalAddr";
    }

    return addr;
}

int getSocketError(int sockFd)
{
    int optval = 0;
    socklen_t optlen = sizeof(optval);

    if (::getsockopt(sockFd, SOL_SOCKET, SO_ERROR, &optval, &optlen) == -1)
    {
        return errno;
    }
    else
    {
        return optval;
    }
    
}
}
}