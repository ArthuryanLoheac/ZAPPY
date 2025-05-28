#pragma once

#include <exception>
#include <string>

namespace GUI {

class GuiException : public std::exception {
 public:
    explicit GuiException(const std::string& message) : _message(message) {}
    virtual ~GuiException() = default;

    const char* what() const noexcept override {
        return _message.c_str();
    }

 protected:
    std::string _message;
};
}  // namespace GUI

