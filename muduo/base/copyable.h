//
// Created by DawenYang on 26/11/2023.
//

#ifndef CPP_SERVER_COPYABLE_H
#define CPP_SERVER_COPYABLE_H

namespace muduo {
    class copyable {
    protected:
        copyable() = default;

        ~copyable() = default;
    };
}// namespace muduo

#endif// CPP_SERVER_COPYABLE_H
