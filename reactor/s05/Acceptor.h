#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

#include <functional>

#include "base/noncopyable.h"
#include "Eventloop.h"
#include "Channel.h"
#include "Socket.h"
#include "Callback.h"

namespace muduo
{
class Acceptor : public noncopyable
{
public:
    Acceptor(Eventloop* loop, const InetAddress& addr);
    void setNewConnectionCallback(const NewConnectionCallback& cb)
    {
        callback_ = cb;
    }

    void listen();
    bool isListen()
    {
        return listening_;
    }
private:
    void handleRead(void);

    Eventloop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback callback_;
    bool listening_;
};
}

#endif