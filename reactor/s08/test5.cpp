#include <stdio.h>

#include <unistd.h>

#include "Eventloop.h"

using namespace muduo;

Eventloop* g_loop;
int g_flag = 0;

#define PRINT printf("%s: pid = %d, flag = %d\n", __FUNCTION__, ::getpid(), g_flag)

void run1(void);
void run2(void);
void run3(void);
void run4(void);

int main(void)
{
    Eventloop loop;
    g_loop = &loop;
    PRINT;
    loop.runAfter(run1, 2.0);
    loop.loop();
}

void run1(void)
{
    g_flag = 1;
    PRINT;
    g_loop->runInLoop(run2);
    // g_flag = 1;
}

void run2(void)
{
    g_flag = 2;
    PRINT;
    g_loop->runInLoop(run3);
    // g_flag = 2;
}

void run3(void)
{
    g_flag = 3;
    PRINT;
    g_loop->runInLoop(run4);
    // g_flag = 3;
}

void run4(void)
{
    g_flag = 4;
    PRINT;
    g_loop->quit();
}