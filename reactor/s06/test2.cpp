//
// Created by zy on 5/26/19.
//


#include "Eventloop.h"
#include "base/Thread.h"

muduo::Eventloop* g_loop;

void threadFunc()
{
    g_loop->loop();
}

int main(void)
{
    muduo::Eventloop loop;
    g_loop = &loop;

    muduo::Thread thread(threadFunc);
    thread.start();
    thread.join();

    return 0;
}
