//
// Created by zy on 3/23/19.
//

#ifndef EPOLLTHREADPOOL_PTHREADPOOL_H
#define EPOLLTHREADPOOL_PTHREADPOOL_H

#include "lock.h"
#include "scopeguard.h"
#include "task.h"

#include <stdio.h>
#include <functional>
#include <deque>
#include <exception>
#include <vector>
#include <atomic>

#include <pthread.h>


class ThreadPool {
public:
    explicit ThreadPool(int threadNum = 20)
    : isStop(false), suspendThreads(0)
    {
        if (threadNum <= 0) {
            perror("thread poll can't be init because the thread number is not greater than zero");
            return;
        }
        pthreads.resize(threadNum);
        for (int i = 0; i < threadNum; ++i) {
            pthreads[i] = new pthread_t{};
        }
    }
    ~ThreadPool()
    {
        stop();
        for (auto threadPtr : pthreads) {
            pthread_join(*threadPtr, nullptr);
            delete threadPtr;
        }
    }

    void start(void)
    {
        for (auto threadPtr : pthreads) {
            if (0 != pthread_create(threadPtr, nullptr, work, this)) {
                throw std::exception{};
            }
        }
    }
    void stop(void)
    {
        isStop = true;
        cond.broadcast();
    }
    bool add(Task&& task)
    {
        lock.lock();
        ScopeGuard guard([&](){ lock.unlock(); });

        tasks.emplace_back(task);
        guard.dismiss();
        lock.unlock();

        if (suspendThreads > 1 && tasks.size() > 3) {
            cond.broadcast();
        } else {
            cond.signal();
        }
        return true;
    }
    bool add(const Task& task)
    {
        Task temTask = task;
        return add(std::move(temTask));
    }
    std::pair<Task, bool > getTask()
    {
        lock.lock();
        ScopeGuard guard([&](){ lock.unlock(); });

        if (!tasks.empty()) {
            Task task = tasks.front();
            tasks.pop_front();
            return {task, true};
        }
        else {
            return {Task{}, false};
        }
    }
private:
    static void* work(void* arg)
    {
        ThreadPool* pool = static_cast<ThreadPool*>(arg);
        pool->run();
        return pool;
    }
    void run(void)
    {
        while(!isStop) {
            auto task = getTask();
            if (!task.second) {
                ++suspendThreads;
                cond.wait();
                --suspendThreads;
            } else {
                task.first();
            }
        }
        printf("thread stop\n");
    }

private:
    MutexLock lock;
//    SemLock   lock;
    CondLock cond;
    std::deque<Task> tasks;
    std::vector<pthread_t*> pthreads;
    std::atomic<int> suspendThreads;
    bool isStop;
};


#endif //EPOLLTHREADPOOL_PTHREADPOOL_H
