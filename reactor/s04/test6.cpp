#include "Eventloop.h"
#include "EventLoopThread.h"
#include "base/CurrentThread.h"

#include <stdio.h>

#include <unistd.h>

using namespace muduo;

void runInLoopThread()
{
    printf("%s: pid = %d, tid = %d\n", __FUNCTION__, ::getpid(), CurrentThread::tid());
}

int main(void)
{
    EventLoopThread eventLoopThread;
    Eventloop* loop = eventLoopThread.startLoop();

    loop->runInLoop(runInLoopThread);
    sleep(1);
    loop->runAfter(runInLoopThread, 2.0);
    sleep(3);
    loop->quit();

    return 0;
}