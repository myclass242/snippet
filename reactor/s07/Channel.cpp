//
// Created by zy on 5/27/19.
//

#include "Channel.h"
#include "base/Logging.h"
#include "Eventloop.h"

#include <poll.h>

using namespace muduo;


const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;


Channel::Channel(Eventloop* loop, int fd)
    : loop_(loop), fd_(fd),
      events_(kNoneEvent), revents_(kNoneEvent),
      index_(-1), eventHandling_(false)
{
}

Channel::~Channel()
{
    assert(!eventHandling_);
    LOG_DEBUG << "Channel::~Channel fd = " << fd_;
}

void Channel::update()
{
    loop_->updateChannel(shared_from_this());
}

void Channel::handleEvent(Timestamp receiveTime)
{
    eventHandling_ = true;
    if (revents_&POLLNVAL) {
        LOG_WARN << "Channel::handle_event() POLLNVAL";
    }

    if ((revents_&(POLLERR | POLLNVAL)) && errorCallback_) {
        errorCallback_();
    }

    if ((revents_&POLLHUP) && !(revents_&POLLIN)) {
        LOG_WARN << "Channel::handleEvent() POLLHUP";
        if (closeCallback_) {
            closeCallback_();
        }
    }

    if ((revents_&(POLLIN | POLLPRI | POLLRDHUP)) && readCallback_) {
        readCallback_(receiveTime);
    }

    if ((revents_&POLLOUT) && writeCallback_) {
        writeCallback_();
    }
    eventHandling_ = false;
}