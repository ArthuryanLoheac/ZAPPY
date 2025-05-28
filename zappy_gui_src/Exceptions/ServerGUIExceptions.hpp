#pragma once

#include <exception>
#include <string>

namespace GUI {

/**
 * @brief Base class for all server GUI exceptions
 */
class ServerGUIException : public std::exception {
public:
    explicit ServerGUIException(const std::string& message) : _message(message) {}
    virtual ~ServerGUIException() = default;

    const char* what() const noexcept override {
        return _message.c_str();
    }

protected:
    std::string _message;
};

/**
 * @brief Exception thrown when parsing commands fails
 */
class CommandParsingException : public ServerGUIException {
public:
    explicit CommandParsingException(const std::string& message)
        : ServerGUIException("Command parsing error: " + message) {}
};

/**
 * @brief Exception thrown when sending data to the server fails
 */
class SendingException : public ServerGUIException {
public:
    explicit SendingException(const std::string& message)
        : ServerGUIException("Sending error: " + message) {}
};

/**
 * @brief Exception thrown when receiving data from the server fails
 */
class ReceivingException : public ServerGUIException {
public:
    explicit ReceivingException(const std::string& message)
        : ServerGUIException("Receiving error: " + message) {}
};

/**
 * @brief Exception thrown when connection to the server fails
 */
class ConnectionException : public ServerGUIException {
public:
    explicit ConnectionException(const std::string& message)
        : ServerGUIException("Connection error: " + message) {}
};

} // namespace GUI
