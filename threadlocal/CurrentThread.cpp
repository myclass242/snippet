//
// Created by zy on 5/25/19.
//


#include "CurrentThread.h"

namespace currentThread
{

std::unordered_map<pid_t, std::string> threadLocal;

thread_local ThreadId _threadId;

}