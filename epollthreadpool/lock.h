//
// Created by zy on 3/23/19.
//

#ifndef EPOLLTHREADPOOL_LOCK_H
#define EPOLLTHREADPOOL_LOCK_H


#include <stdio.h>
#include <functional>

#include <pthread.h>
#include <semaphore.h>

#include "scopeguard.h"

class SemLock {
public:
    SemLock(int value = 1)
    {
        if (0 != sem_init(&sem, 0, value) ) {
            perror("init semaphore failed");
        }
    }
    ~SemLock()
    {
        sem_destroy(&sem);
    }
    bool wait()
    {
        return sem_wait(&sem) == 0;
    }
    bool post()
    {
        return sem_post(&sem) == 0;
    }
private:
    sem_t sem;
};

class MutexLock {
public:
    MutexLock()
    {
        if (0 != pthread_mutex_init(&mutex, nullptr)) {
            perror("init mutex failed");
        }
    }
    ~MutexLock()
    {
        pthread_mutex_destroy(&mutex);
    }
    bool lock()
    {
        return 0 == pthread_mutex_lock(&mutex);
    }
    bool unlock()
    {
        return 0 == pthread_mutex_unlock(&mutex);
    }
private:
    pthread_mutex_t mutex;
};

class CondLock {
public:
    CondLock()
    {
        if (0 != pthread_mutex_init(&mutex, nullptr)) {
            perror("mutex init failed");
            return;
        }
        if (0 != pthread_cond_init(&cond, nullptr)) {
            pthread_mutex_destroy(&mutex);
            perror("condition init failed");
        }
    }
    ~CondLock()
    {
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }
    bool wait()
    {
        int ans = 0;
        pthread_mutex_lock(&mutex);
        ScopeGuard guard(std::bind(pthread_mutex_unlock, &mutex));
        ans = pthread_cond_wait(&cond, &mutex);
        return ans == 0;
    }
    bool signal()
    {
        return pthread_cond_signal(&cond) == 0;
    }
    bool broadcast()
    {
        return pthread_cond_broadcast(&cond) == 0;
    }
private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};
#endif //EPOLLTHREADPOOL_LOCK_H
