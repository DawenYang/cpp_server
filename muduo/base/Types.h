//
// Created by DawenYang on 28/11/2023.
//

#ifndef CPP_SERVER_TYPES_H
#define CPP_SERVER_TYPES_H

#include <stdint.h>
#include <string.h>// memset
#include <string>

#ifdef NDEBUG
#include <assert.h>
#endif

namespace muduo {
    using std::string;

    inline void memZero(void *p, size_t n) {
        memset(p, 0, n);
    }

    template<typename To, typename From>
    inline To implicit_cast(From const &f) {
        return f;
    }

    template<typename To, typename From>
    inline To down_cast(From *f) {
        if (false) {
            implicit_cast<From *, To>(0);
        }
    }

#if !defined(NDEBUG) && !defined(GOOGLE_PROTOBUF_NO_RTTI)
    assert(f == NULL || dynamic_cast<To>(f) != NULL);
#endif
}// namespace muduo

#endif//CPP_SERVER_TYPES_H
