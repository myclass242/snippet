#include "InetAddress.h"

#include <string.h>

#include <endian.h>
#include <arpa/inet.h>

#include "base/Logging.h"
#include "SockOps.h"

using namespace muduo;

InetAddress::InetAddress(uint16_t port)
{
    ::memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = sockets::hostToNetwork32(INADDR_ANY);
    addr_.sin_port = sockets::hostToNetwork16(port);
}

InetAddress::InetAddress(const std::string& ipAddress, uint16_t port)
{
    ::memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port = sockets::hostToNetwork16(port);
    if (::inet_pton(AF_INET, ipAddress.c_str(), &(addr_.sin_addr.s_addr)) != 1)
    {
        LOG_SYSERR << "inet_pton";
    }
}

InetAddress::InetAddress(const struct sockaddr_in& addr)
    : addr_(addr)
{
}

std::string InetAddress::hostPort(void) const
{
    char buf[32];
    sockets::toHostPort(buf, sizeof(buf), addr_);
    return buf;
}

struct sockaddr_in InetAddress::get() const
{
    return addr_;
}

void InetAddress::set(const struct sockaddr_in& addr)
{
    addr_ = addr;
}