#pragma once

#include <exception>
#include <string>

namespace AI {

class AIException : public std::exception {
 public:
    explicit AIException(const std::string& message) : _message(message) {}
    virtual ~AIException() = default;

    const char* what() const noexcept override {
        return _message.c_str();
    }

 protected:
    std::string _message;
};
}  // namespace AI

