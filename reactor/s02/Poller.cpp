//
// Created by zy on 5/27/19.
//

#include "Poller.h"

#include "base/Logging.h"
#include "Channel.h"


using namespace muduo;

Poller::Poller(muduo::Eventloop *loop)
    : ownLoop_(loop)
{}

Poller::~Poller()
{}

Timestamp Poller::poll(int timeoutMs, muduo::Poller::ChannelList *activeChannels)
{
    int numEvents = ::poll(&(*pollfds_.begin()), pollfds_.size(), timeoutMs);
    Timestamp now(Timestamp::now());

    if (numEvents > 0) {
        LOG_TRACE << numEvents << " events happend";
        fillActiveChannels(numEvents, activeChannels);
    } else if (numEvents == 0) {
        LOG_TRACE << "Nothing happend";
    } else {
        LOG_SYSERR << "Poller::poll()";
    }

    return now;
}

void Poller::fillActiveChannels(int numEvents, muduo::Poller::ChannelList *activeChannels)
{
    for (const auto& pollfd : pollfds_) {
        if (numEvents <= 0) {
            break;
        }

        if (pollfd.revents > 0) {
            --numEvents;
            ChannelMap ::const_iterator ch = channels_.find(pollfd.fd);
            assert(ch != channels_.end());
            Channel* channel = ch->second;
            assert(channel->fd() == pollfd.fd);
            channel->setReturnEvents(pollfd.revents);
            activeChannels->push_back(channel);
        }
    }
}

void Poller::updateChannel(muduo::Channel *channel)
{
    assertInLoopThread();
    LOG_TRACE << "fd = " << channel->fd() << " events = " << channel->events();
    if (channel->index() < 0) {
        // a new one update to pollfds_
        assert(channels_.find(channel->fd()) == channels_.end());
        struct pollfd newPfd;
        newPfd.fd = channel->fd();
        newPfd.events = channel->events();
        newPfd.revents = 0;
        pollfds_.push_back(std::move(newPfd));
        int index = pollfds_.size() - 1;
        channel->setIndex(index);
        channels_[channel->fd()] = channel;
    } else {
        assert(channels_.find(channel->fd()) != channels_.end());
        assert(channels_[channel->fd()] == channel);
        int index = channel->index();
        assert(index >= 0 && index < static_cast<int>(pollfds_.size()));
        auto& pfd = pollfds_[index];
        assert(pfd.fd == channel->fd() || pfd.fd == -1);
        pfd.events = static_cast<short int>(channel->events());
        pfd.revents = 0;
        if (channel->isNoneEvent()) {
            pfd.fd == -1;
        }

    }
}
