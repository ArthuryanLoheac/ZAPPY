/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Arguments
*/

#pragma once

#include <exception>
#include <string>

namespace AI {

// Base of all factory exceptions
class FactoryException : public std::exception {
 public:
    /**
     * @brief Constructor
        * @param message The error message
        */
    explicit FactoryException(const std::string &message) :
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

class ArgumentsException : public FactoryException {
 public:
    explicit ArgumentsException(const std::string &message) :
        FactoryException("Arguments error: " + message) {}
};

class ChildProcessException : public FactoryException {
 public:
    explicit ChildProcessException(const std::string &message) :
        FactoryException("Child process error: " + message) {}
};

}  // namespace AI
