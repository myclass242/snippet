//
// Created by zy on 5/27/19.
//

#ifndef POLLER_H
#define POLLER_H

#include <vector>
#include <map>

#include <poll.h>

#include "base/noncopyable.h"
#include "Eventloop.h"
#include "base/Timestamp.h"

namespace muduo {
class Channel;

/*
 *  IO Multiplexing with poll
 *  This class doesn't own the Channel objects
 * */
class Poller : public noncopyable
{
public:
    using ChannelList = std::vector<Channel*>;

    Poller(Eventloop* loop);
    ~Poller();

    /*
     * Polls the IO events
     * Must be called in the loop thread
     * */
    Timestamp poll(int timeoutMs, ChannelList* activeChannels);

    /*
     * Changes the interested IO events
     * Must be called int the loop thread
     * */
    void updateChannel(Channel* channel);

    void assertInLoopThread()
    {
        ownLoop_->assertInLoopThread();
    }
private:
    void fillActiveChannels(int numEvents, ChannelList* activeChannels);

    using ChannelMap = std::map<int, Channel*>;
    using PollfdList = std::vector<struct pollfd>;

    Eventloop* ownLoop_;
    PollfdList pollfds_;
    ChannelMap channels_;
};
}

#endif //S01_POLLER_H
