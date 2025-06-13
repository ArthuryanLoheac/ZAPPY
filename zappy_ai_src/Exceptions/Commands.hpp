#pragma once

#include <exception>
#include <string>

namespace AI {

// Base of all factory exceptions
class CommandsException : public std::exception {
 public:
    /**
     * @brief Constructor
        * @param message The error message
        */
    explicit CommandsException(const std::string &message) :
        _message(message) {}

    /**
     * @brief Get the error message
     * @return const char* The error message
     */
    const char *what() const noexcept override {
        return _message.c_str();
    }

 protected:
    std::string _message;
};

class CommandNotFoundException : public CommandsException {
 public:
    explicit CommandNotFoundException(const std::string &command) :
        CommandsException("Command not found: " + command) {}
};

class CommandArgumentsException : public CommandsException {
 public:
    explicit CommandArgumentsException(const std::string &command,
        const std::string &message) :
        CommandsException("Invalid arguments for command '" + command + "': " +
            message) {}
};

}  // namespace AI
