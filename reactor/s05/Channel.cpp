//
// Created by zy on 5/27/19.
//

#include "Channel.h"
#include "base/Logging.h"

#include <poll.h>

using namespace muduo;


const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;


Channel::Channel(Eventloop* loop, int fd)
    : loop_(loop), fd_(fd),
      events_(kNoneEvent), revents_(kNoneEvent),
      index_(-1)
{
}

void Channel::update()
{
    loop_->updateChannel(this);
}

void Channel::handleEvent()
{
    if (revents_&POLLNVAL) {
        LOG_WARN << "Channel::handle_event() POLLNVAL";
    }

    if ((revents_&(POLLERR | POLLNVAL)) && errorCallback) {
        errorCallback();
    }

    if ((revents_&(POLLIN | POLLPRI | POLLRDHUP)) && readCallback_) {
        readCallback_();
    }

    if ((revents_&POLLOUT) && writeCallback_) {
        writeCallback_();
    }
}