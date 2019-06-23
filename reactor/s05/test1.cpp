//
// Created by zy on 5/26/19.
//


#include "Eventloop.h"

#include <stdio.h>

#include <unistd.h>

#include "base/Thread.h"

void threadFunc()
{
    printf("threadFunc: pid = %d, tid = %d\n", getpid(), muduo::CurrentThread::tid());

    muduo::Eventloop loop;
    loop.loop();
}

int main(void)
{
    printf("main: pid = %d, tid = %d\n", getpid(), muduo::CurrentThread::tid());
    muduo::Eventloop loop;

    muduo::Thread thread(threadFunc);
    thread.start();

    loop.loop();

    pthread_exit(NULL);
}