//
// Created by zy on 3/23/19.
//

#ifndef EPOLLTHREADPOOL_TASK_H
#define EPOLLTHREADPOOL_TASK_H

#include <functional>

class Task {
    using Func = std::function<void(void)>;
public:
    Task()
    : func_(dummy)
    {}
    explicit Task(const Func& f)
    : func_(f)
    {}
    explicit Task(Func&& f)
    : func_(std::move(f))
    {}
    Task(const Task& rhs)
    : func_(rhs.func_)
    {}
    Task(Task&& rhs)
    : func_(std::move(rhs.func_))
    {
        rhs.func_ = dummy;
    }
    Task& operator=(const Task& rhs)
    {
        func_ = rhs.func_;
        return *this;
    }
    Task& operator=(Task&& rhs)
    {
        func_ = std::move(rhs.func_);
        rhs.func_ = dummy;
        return *this;
    }

    void operator()(void)
    {
        func_();
    }
private:
    static void dummy(void) {}

private:
    Func func_;
};

#endif //EPOLLTHREADPOOL_TASK_H
