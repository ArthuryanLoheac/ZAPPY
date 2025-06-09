#pragma once

#include <exception>
#include <string>

namespace AI {
class AIException : public std::exception {
 protected:
    std::string _message;

 public:
    explicit AIException(const std::string &message) : _message(message) {}
    virtual ~AIException() = default;
    const char *what() const noexcept override {
        return _message.c_str();
    }
};

class DataManagerException : public AIException {
 public:
    explicit DataManagerException(const std::string &message) :
        AIException("DataManager: " + message) {}
    virtual ~DataManagerException() = default;
};

class ConnectionException : public DataManagerException {
 public:
    explicit ConnectionException(const std::string &message) :
        DataManagerException("Connection error: " + message) {}
};

class ParseException : public DataManagerException {
 public:
    explicit ParseException(const std::string &message) :
        DataManagerException("Parse error: " + message) {}
};

class TimeoutException : public DataManagerException {
 public:
    explicit TimeoutException(const std::string &message) :
        DataManagerException(message) {}
};

class InvalidDataException : public DataManagerException {
 public:
    explicit InvalidDataException(const std::string &message) :
        DataManagerException("Invalid data: " + message) {}
};
}  // namespace AI
