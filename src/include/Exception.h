//
// Created by Dawen on 1/19/23.
//

#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

enum  ExceptionType {
    INVALID = 0,
    INVALID_SOCKET = 1,
};

class Exception {
public:
    explicit Exception(const std::string &message) : std::runtime_error(message), type_(ExceptionType::INVALID) {
        std::string exception_message = "Message :: " + message + "\n";
        std::cerr<<exception_message;
    }

    Exception(ExceptionType type, const std::string &message) : std::runtime_error(message), type_(type) {
        std::string exception_message = "Exception Type :: " + ExceptionTypeToString(type_) + "\nMessage :: " + message + "\n";
        std::cerr << exception_message;
    }

    static std::string ExceptionTypeToString(ExceptionType type) {
        switch (type) {
            case ExceptionType::INVALID:
                return "Invalid";
            case ExceptionType::INVALID_SOCKET:
                return "Invalid Socket";
            default:
                return "Unknown";
        }
    }

private:
    ExceptionType type_;
};
