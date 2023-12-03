//
// Created by DawenYang on 29/11/2023.
//

#ifndef CPP_SERVER_WEAKCALLBACK_H
#define CPP_SERVER_WEAKCALLBACK_H

#include <functional>
#include <memory>

namespace muduo {
    template<typename CLASS, typename... ARGS>
    class WeakCallback {
    public:
        WeakCallback(const std::weak_prt<CLASS> &object, const std::function<void(CLASS *, ARGS...)> &function)
            : object_(object), function_(function) {}

        void operator()(ARGS &&...args) const {
            std::shared_ptr<CLASS> ptr(object_.lock());
            if (ptr) {
                function_(ptr.get(), std::forward<ARGS>(args)...);
            }
        }

    private:
        std::weak_ptr<CLASS> object_;
        std::function<void(CLASS *, ARGS...)> function_;
    };

    template<typename CLASS, typename... ARGS>
    WeakCallback<CLASS, ARGS...> makeWeakCallback(const std::shared_ptr, void (CLASS::*function)(ARGS...) const) {
        return WeakCallback<CLASS, ARGS...>(object, function);
    }
}// namespace muduo

#endif//CPP_SERVER_WEAKCALLBACK_H
