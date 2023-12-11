//
// Created by dawen on 12/11/2023.
//

#ifndef MUDUO_MUDUO_BASE_SINGLETON_H_
#define MUDUO_MUDUO_BASE_SINGLETON_H_

#include "muduo/base/noncopyable.h"

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

namespace muduo {
    namespace detail {
        template<typename T>
        struct has_no_destroy{
            template<typename C>
            static char test(decltype(&C::no_destroy));
            template<typename C>
            static int32_t test(...);
            const static bool value = sizeof(test<T>(0)) == 1;
        };

        template<typename T>
        class Singleton : noncopyable {
        public:
            Singleton() = delete;
            ~Singleton() = delete;

            static T& instance() {
                pthread_once(&ponce_, &Singleton::init);
                assert(value_ != NULL);
                return *value_;
            }

        private:
            static void init() {
                value_ = new T();
                if (!detail::has_no_destroy<T>::value) {
                    ::atexit(destroy);
                }
            }

            static void destroy() {
                typedef char T_must_be_complete_type[sizeof(T) == 0? -1: 1];
                T_must_be_complete_type dummy; (void) dummy;

                delete value_;
                value_ = NULL;
            }
        private:
            static pthread_once_t ponce_;
            static T* value_;
        };
    }
}

#endif//MUDUO_MUDUO_BASE_SINGLETON_H_