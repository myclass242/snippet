#include "InetAddress.h"

#include <string.h>

#include <endian.h>
#include <arpa/inet.h>

#include "base/Logging.h"
#include "base/"

using namespace muduo;

InetAddress::InetAddress(uint16_t port)
{
    ::memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = htobe32(INADDR_ANY);
    addr_.sin_port = htobe16(port);
}

InetAddress::InetAddress(const std::string& ipAddress, unit16_t port)
{
    ::memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;
    addr_.sin_port = htobe16(port);
    if (::inet_pton(AF_INET, ipAddress.c_str(), &(addr_.sin_addr.s_addr)) != 1)
    {
        LOG_SYSERR << "inet_pton";
    }
}

InetAddress::InetAddress(const struct sockaddr_in& addr)
    : addr_(addr)
{
}

std::string hostPort(void) const
{
    
}