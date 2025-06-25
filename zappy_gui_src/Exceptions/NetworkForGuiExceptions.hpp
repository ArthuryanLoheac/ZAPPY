#pragma once

#include <exception>
#include <string>

namespace GUI {

/**
 * @brief Base class for all server GUI exceptions
 */
class NetworkForGuiException : public std::exception {
 public:
    explicit NetworkForGuiException(const std::string& message) :
        _message(message) {}
    virtual ~NetworkForGuiException() = default;

    const char* what() const noexcept override {
        return _message.c_str();
    }

 protected:
    std::string _message;
};

/**
 * @brief Exception thrown when parsing commands fails
 */
class CommandParsingException : public NetworkForGuiException {
 public:
    explicit CommandParsingException(const std::string& message)
        : NetworkForGuiException("Command parsing error: " + message) {}
};

/**
 * @brief Exception thrown when sending data to the server fails
 */
class SendingException : public NetworkForGuiException {
 public:
    explicit SendingException(const std::string& message)
        : NetworkForGuiException("Sending error: " + message) {}
};

/**
 * @brief Exception thrown when receiving data from the server fails
 */
class ReceivingException : public NetworkForGuiException {
 public:
    explicit ReceivingException(const std::string& message)
        : NetworkForGuiException("Receiving error: " + message) {}
};

}  // namespace GUI
