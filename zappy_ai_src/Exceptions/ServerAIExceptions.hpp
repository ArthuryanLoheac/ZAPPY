#pragma once

#include <exception>
#include <string>

namespace AI {

/**
 * @brief Base class for all server AI exceptions
 */
class ServerAIException : public std::exception {
 public:
    explicit ServerAIException(const std::string& message) :
        _message(message) {}
    virtual ~ServerAIException() = default;

    const char* what() const noexcept override {
        return _message.c_str();
    }

 protected:
    std::string _message;
};

/**
 * @brief Exception thrown when parsing commands fails
 */
class CommandParsingException : public ServerAIException {
 public:
    explicit CommandParsingException(const std::string& message)
        : ServerAIException("Command parsing error: " + message) {}
};

/**
 * @brief Exception thrown when sending data to the server fails
 */
class SendingException : public ServerAIException {
 public:
    explicit SendingException(const std::string& message)
        : ServerAIException("Sending error: " + message) {}
};

/**
 * @brief Exception thrown when receiving data from the server fails
 */
class ReceivingException : public ServerAIException {
 public:
    explicit ReceivingException(const std::string& message)
        : ServerAIException("Receiving error: " + message) {}
};

}  // namespace AI
