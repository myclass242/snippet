//
// Created by zy on 5/26/19.
//

#include <stdio.h>

#include <iostream>

#include "Eventloop.h"
#include "Channel.h"
#include "base/Timestamp.h"

#include <sys/timerfd.h>
#include <unistd.h>

muduo::Eventloop* g_loop;

void timeout(int timerfd)
{
    printf("Timeout\n");
    std::cout << muduo::Timestamp::now().toString() << '\n';
    char buf[1024];
    read(timerfd, buf, 1024);
//    g_loop->quit();
}

int main(void)
{
    muduo::Eventloop loop;
    g_loop = &loop;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    muduo::Channel channel(&loop, timerfd);
    channel.setReadCallback([&](){timeout(timerfd);});
    channel.enableReading();

    struct itimerspec howlong;
    memset(&howlong, 0, sizeof(howlong));
    howlong.it_value.tv_sec = 2;
    howlong.it_interval.tv_sec = 3;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);

    loop.loop();

    ::close(timerfd);

    return 0;
}
