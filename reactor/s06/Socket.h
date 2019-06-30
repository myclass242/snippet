#ifndef SOCKET_H_
#define SOCKET_H_

#include "base/noncopyable.h"

namespace muduo
{
class InetAddress;
class Socket : public noncopyable
{
public:
    explicit Socket(int socket)
        : sockfd_(socket)
    {}
    ~Socket();

    int fd() const
    {
        return sockfd_;
    }

    void bind(const InetAddress& addr) const;
    void listen(void) const;
    /// On success, returns a non-negative integer that is
    /// a descriptor for the accepted socket, which has been
    /// set to non-blocking and close-on-exec. *peeraddr is assigned.
    /// On error, -1 is returned, and *peeraddr is untouched.
    int accept(InetAddress* peer);

    void setReuseAddr(bool on);
private:
    int sockfd_;
};
}

#endif