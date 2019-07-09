//
// Created by zy on 5/27/19.
//

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include <functional>
#include <memory>

#include "base/noncopyable.h"
#include "base/Timestamp.h"

namespace muduo
{
class Eventloop;
class Channel : public noncopyable, public std::enable_shared_from_this<Channel>
{
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(Eventloop* loop, int fd);
    ~Channel();

    void handleEvent(Timestamp receiveTime);

    void setReadCallback(const ReadEventCallback& rcb)
    {
        readCallback_ = rcb;
    }
    void setWriteCallback(const EventCallback& wcb)
    {
        writeCallback_ = wcb;
    }
    void setErrorCallback(const EventCallback& ecb)
    {
        errorCallback_ = ecb;
    }
    void setCloseCallback(const EventCallback& ccb)
    {
        closeCallback_ = ccb;
    }

    int fd() const noexcept
    {
        return fd_;
    }

    int events() const noexcept
    {
        return events_;
    }

    void setReturnEvents(int returnEvents) noexcept
    {
        revents_ = returnEvents;
    }

    bool isNoneEvent() const noexcept
    {
        return events_ == kNoneEvent;
    }

    void enableReading()
    {
        events_ |= kReadEvent;
        update();
    }

    void enableWriting()
    {
        events_ |= kWriteEvent;
        update();
    }

    void disableWriting()
    {
        events_ &= ~kWriteEvent;
        update();
    }

    bool isWriting()
    {
        return events_ & kWriteEvent;
    }

    void disableAll()
    {
        events_ = kNoneEvent;
        update();
    }

    //for poll
    int index() const noexcept
    {
        return index_;
    }
    void setIndex(int index)
    {
        index_ = index;
    }

    Eventloop* ownloop() noexcept
    {
        return loop_;
    }
private:
    void update();

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    Eventloop* loop_;
    int fd_;
    int events_;    // 关心的IO事件
    int revents_;   // 目前活动的事件
    int index_; // used by Poller
    bool eventHandling_;

    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback_;
    EventCallback closeCallback_;
};

using ChannelPtr = std::shared_ptr<Channel>;
}


#endif //CHANNEL_H_
