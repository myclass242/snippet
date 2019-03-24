//
// Created by zy on 3/23/19.
//

#ifndef EPOLLTHREADPOOL_EPOLLSERVER_H
#define EPOLLTHREADPOOL_EPOLLSERVER_H

#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

class Epman
{
public:
    Epman() {}
    Epman(const Epman&) = delete;
    Epman(Epman&&) = delete;
    ~Epman()
    {
        close(epfd_);
    }
    int init()
    {
        epfd_ = epoll_create(1);
        if (epfd_ == -1) {
            perror("create epoll failed");
        }
        return epfd_;
    }
    int getEpoll()
    {
        return epfd_;
    }
    static int nonblock(int fd)
    {
        int oldOpt = fcntl(fd, F_GETFL);
        int newOpt = oldOpt | O_NONBLOCK;
        fcntl(fd, F_SETFL, newOpt);
        return oldOpt;
    }
    static void add(int epollFd, int fd, epoll_event* event)
    {
        if (-1 == epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, event)) {
            perror("add file descriptor to epoll failed");
        }
        Epman::nonblock(fd);
    }
    static void del(int epollFd, int fd)
    {
        epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, nullptr);
    }
    static void mod(int epollFd, int fd, epoll_event* event)
    {
        epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, event);
    }
    int wait(epoll_event* events, int maxevents)
    {
        return epoll_wait(epfd_, events, maxevents, -1);
    }
private:
    int epfd_;
};

#endif //EPOLLTHREADPOOL_EPOLLSERVER_H
