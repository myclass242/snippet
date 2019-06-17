#ifndef INET_ADDRESS_H_
#define INET_ADDRESS_H_

#include <stdint.h>

#include <string>

#include <netinet/in.h>

#include "base/copyable.h"

namespace muduo
{
class InetAddress : public copyable
{
public:
    explicit InetAddress(uint16_t port);
    InetAddress(const std::string& ipAddress, uint16_t port);
    InetAddress(const struct sockaddr_in& addr);

    std::string hostPort(void) const;
    struct sockaddr_in get(void) const;
    void set(const struct sockaddr_in& addr);
private:
    struct sockaddr_in addr_;
};
}

#endif