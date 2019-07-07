#include <stdio.h>

#include <functional>

#include "Eventloop.h"
#include "base/CurrentThread.h"

#include <sys/types.h>
#include <unistd.h>

using namespace muduo;

Eventloop* g_loop = nullptr;
int cnt = 0;

void printId(void)
{
    printf("Pid:%d tid:%d\n", ::getpid(), CurrentThread::tid());
    printf("now %s\n", Timestamp::now().toString().c_str());
}

void print(const char* msg)
{
    printf("message %s : %s\n", msg, Timestamp::now().toString().c_str());
    if (++cnt == 20) 
    {
        g_loop->quit();
    }
}

int main(void)
{
    Eventloop loop;
    g_loop = &loop;

    printId();

    g_loop->runAfter(std::bind(print, "once1"), 1.0);
    g_loop->runAfter(std::bind(print, "once1.5"), 1.5);
    g_loop->runAfter(std::bind(print, "once2.5"), 2.5);
    g_loop->runAfter(std::bind(print, "once3.5"), 3.5);

    loop.runEvery(std::bind(print, "every2"), 2.0);
    loop.runEvery(std::bind(print, "every3"), 3.0);

    loop.loop();

    return 0;
}