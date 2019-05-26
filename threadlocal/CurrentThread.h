//
// Created by zy on 5/25/19.
//

#ifndef THREADLOCAL_CURRENTTHREAD_H
#define THREADLOCAL_CURRENTTHREAD_H

#include <unordered_map>
#include <string>
#include <iostream>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

namespace currentThread
{
extern std::unordered_map<pid_t, std::string> threadLocal;

struct ThreadId
{
    ThreadId()
    {
        threadId_ = static_cast<pid_t>(::syscall(SYS_gettid));
        threadLocal[threadId_] = std::to_string(threadId_);
        std::cout << "construct thread local:" << threadId_ << std::endl;
    }
    ~ThreadId()
    {
        threadLocal.erase(threadId_);
        std::cout << "destruct thread local: " << threadId_ << std::endl;
    }

    pid_t threadId_;
};

extern thread_local ThreadId _threadId;

}

#endif //THREADLOCAL_CURRENTTHREAD_H
