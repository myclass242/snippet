//
// Created by zy on 5/27/19.
//

#ifndef S01_CHANNEL_H
#define S01_CHANNEL_H

#include <functional>

#include "Eventloop.h"

namespace muduo
{
class Channel : public noncopyable{
public:
    using EventCallback = std::function<void()>;

    Channel(Eventloop* loop, int fd);

    void handleEvent();

    void setReadCallback(const EventCallback& rcb)
    {
        readCallback_ = rcb;
    }
    void setWriteCallback(const EventCallback& wcb)
    {
        writeCallback_ = wcb;
    }
    void setErrorCallback(const EventCallback& ecb)
    {
        errorCallback = ecb;
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

    EventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback errorCallback;
};
}


#endif //S01_CHANNEL_H
