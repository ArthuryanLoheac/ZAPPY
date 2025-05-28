#pragma once

#include <exception>
#include <string>

class GuiException : public std::exception {
 protected:
    std::string _message;

 public:
    explicit GuiException(const std::string &message) : _message(message) {}
    virtual ~GuiException() = default;
    const char *what() const noexcept override {
        return _message.c_str();
    }
};

class DataManagerException : public GuiException {
 public:
    explicit DataManagerException(const std::string &message) :
        GuiException("DataManager: " + message) {}
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
