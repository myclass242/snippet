#include <iostream>
#include <thread>
#include <iostream>

#include "CurrentThread.h"

int main()
{
    auto threadFn = [](){
        currentThread::threadLocal[currentThread::_threadId.threadId_];
//        sleep(5);
        std::cout << "thread" << std::endl;
    };

    std::thread threads[5];
    for (int k = 0; k < 5; ++k) {
        threads[k] = std::thread(threadFn);
        sleep(5);
    }

    for (int k = 0; k < 5; ++k) {
        threads[k].join();
    }


    sleep(10);
    return 0;
}