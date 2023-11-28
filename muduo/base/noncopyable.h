//
// Created by DawenYang on 27/11/2023.
//

#ifndef CPP_SERVER_NONCOPYABLE_H
#define CPP_SERVER_NONCOPYABLE_H

namespace muduo {
    class noncopyable {
    public:
        noncopyable(const noncopyable &) = delete;
        void operator=(const noncopyable &) = delete;

    protected:
        noncopyable() = default;
        ~noncopyable() = default;
    };
}  // namespace muduo
#endif  // CPP_SERVER_NONCOPYABLE_H
