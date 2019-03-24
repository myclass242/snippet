//
// Created by zy on 3/23/19.
//

#ifndef EPOLLTHREADPOOL_SCOPEGUARD_H
#define EPOLLTHREADPOOL_SCOPEGUARD_H

#include <utility>

template <typename F>
class ScopeGuard{
public:
    ScopeGuard() = delete;
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    explicit ScopeGuard(const F& f)
    : func_(f), dismiss_(false)
    {}
    explicit ScopeGuard(F&& f)
    : func_(std::move(f)), dismiss_(false)
    {}
    ScopeGuard(ScopeGuard&& rhs)
    : func_(std::move(rhs.func_)), dismiss_(rhs.dismiss_)
    {}
    ~ScopeGuard()
    {
        if (!dismiss_) {
            func_();
        }
    }
    void dismiss()
    {
        dismiss_ = true;
    }

private:
    F func_;
    bool dismiss_;
};

#endif //EPOLLTHREADPOOL_SCOPEGUARD_H
