//
// Created by zy on 3/23/19.
//

#ifndef EPOLLTHREADPOOL_SERVER_H
#define EPOLLTHREADPOOL_SERVER_H


#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>

#include "epollmanager.h"
#include "pthreadpool.h"

class Serv {
public:
    Serv() = delete;
    explicit Serv(int p, int threadNumber = 10)
        : port(p), pool(threadNumber), isStop(false)
    {}
    Serv(const Serv&) = delete;
    Serv(Serv&&) = delete;
    ~Serv() {}
    void init()
    {
        memset(&bindAddr, 0, sizeof(bindAddr));
        bindAddr.sin_family = AF_LOCAL;
        bindAddr.sin_port = htons(port);
        bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servFd = socket(AF_LOCAL, SOCK_STREAM, 0);
        if (servFd == -1) {
            perror("create socket failed");
            return;
        }
        int ret = bind(servFd, (struct sockaddr*)(&bindAddr), sizeof(bindAddr));
        if (ret == -1) {
            perror("bind socket failed");
            return;
        }
        ret = listen(servFd, 10);
        if (ret == -1) {
            perror("listen socket failed");
            return;
        }


        if (epollMgr.init() == -1) {
            return;
        }
    }
    void start(void)
    {
        pool.start();

        epoll_event servEvent;
        ctlEvent(servFd, false, &servEvent);
        Epman::add(epollMgr.getEpoll(), servFd, &servEvent);

        while(!isStop) {
            int ret = epollMgr.wait(events, MAX_EVENT);
            if (ret == -1) {
                perror("epoll wait failed");
                break;
            }

            for (int i = 0; i < ret; ++i) {
                int fd = events[i].data.fd;
                if (fd == servFd) {
                    if (addConnection() == -1) {
                        continue;
                    }
                } else if (events[i].events & EPOLLIN) {
                    readConnection(fd);
                } else {
                    perror("unkonw error");
                }
            }
        }

        close(servFd);
        pool.stop();
    }
    void stop()
    {
        isStop = true;
    }
private:
    void ctlEvent(int fd, int oneshot, epoll_event* event)
    {
        event->data.fd = fd;
        event->events = EPOLLIN | EPOLLET;
        if (oneshot) {
            event->events |= EPOLLONESHOT;
        }
    }
    int addConnection(void)
    {
        sockaddr_in clientAddr;
        memset(&clientAddr, 0, sizeof(clientAddr));
        socklen_t len = sizeof(clientAddr);
        int confd = accept(servFd, (sockaddr*)&clientAddr, &len);
        if (confd == -1) {
            perror("accept new connection failed");
            return -1;
        }
        epoll_event conEvent;
        ctlEvent(confd, false, &conEvent);
        Epman::add(epollMgr.getEpoll(), confd, &conEvent);

        return 0;
    }
    void readConnection(int fd)
    {
        char buff[1024];
        readagain:
        memset(buff, 0, sizeof(buff));
        int ret = read(fd, buff, sizeof(buff) - 1);
        if (ret == 0) {
            // fd is closed, so remove from epoll
            Epman::del(epollMgr.getEpoll(), fd);
            shutdown(fd, SHUT_RDWR);
        } else if (ret > 0) {
            Task task([=](){
                std::string str{buff};
                str += "somedata\n";
                write(fd, str.c_str(), str.length());
            });
            pool.add(std::move(task));
        } else {
            // error
            if (errno == EAGAIN) {
                printf("read again\n");
                goto readagain;
            }
        }
    }
private:
    static const int MAX_EVENT = 1024;
    Epman epollMgr;
    epoll_event events[MAX_EVENT];
    ThreadPool pool;
    int servFd;
    int port;
    sockaddr_in bindAddr;
    bool isStop;
};

#endif //EPOLLTHREADPOOL_SERVER_H
