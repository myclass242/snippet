//
// Created by zy on 5/26/19.
//

#include "Eventloop.h"

#include <poll.h>

#include "base/Logging.h"
#include "Poller.h"
#include "Channel.h"

using namespace muduo;


thread_local Eventloop* t_loopInThisThread = nullptr;
namespace
{
const int kPollTimeMs = 500;
}

Eventloop::Eventloop()
    : looping_(false), threadId_(CurrentThread::tid()),
      quit_(false)
{
    poller_.reset(new Poller(this));
    LOG_TRACE << "Eventloop created " << this << " in thread " << threadId_;

    if (t_loopInThisThread)
    {
        LOG_FATAL << "Another Eventloop " << t_loopInThisThread << " exists in this thread " << threadId_;
    }
    else
    {
        t_loopInThisThread = this;
    }
}

Eventloop::~Eventloop()
{
    assert(!looping_);
    t_loopInThisThread = nullptr;
}

void Eventloop::abortNotInLoopThread()
{
    LOG_ERROR << "Not in loop thread";
    abort();
}

Eventloop* Eventloop::getEventloopOfCurrentThread()
{
    return t_loopInThisThread;
}

void Eventloop::loop()
{
    assert(!looping_);
    assertInLoopThread();

    looping_ = true;
    quit_ = false;

    while (!quit_) {
	activeChannels_.clear();
        poller_->poll(kPollTimeMs, &activeChannels_);
        for (auto& channel : activeChannels_) {
            channel->handleEvent();
        }
    }

    LOG_TRACE << "Eventloop " << this << " stop looping";
    looping_ = false;
}

void Eventloop::updateChannel(muduo::Channel *channel)
{
    assert(channel->ownloop() == this);
    assertInLoopThread();

    poller_->updateChannel(channel);
}
