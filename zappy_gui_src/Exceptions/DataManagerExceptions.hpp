#pragma once

#include <exception>
#include <string>

namespace GUI {

/**
 * @brief Base class for GUI-related exceptions.
 */
class GuiException : public std::exception {
 protected:
    std::string _message; /**< Exception message. */

 public:
    /**
     * @brief Constructs a new GuiException object.
     *
     * @param message The exception message.
     */
    explicit GuiException(const std::string &message) : _message(message) {}

    /**
     * @brief Virtual destructor.
     */
    virtual ~GuiException() = default;

    /**
     * @brief Gets the exception message.
     *
     * @return const char* The exception message.
     */
    const char *what() const noexcept override {
        return _message.c_str();
    }
};

/**
 * @brief Exception class for DataManager-related errors.
 */
class DataManagerException : public GuiException {
 public:
    /**
     * @brief Constructs a new DataManagerException object.
     *
     * @param message The exception message.
     */
    explicit DataManagerException(const std::string &message) :
        GuiException("DataManager: " + message) {}

    /**
     * @brief Virtual destructor.
     */
    virtual ~DataManagerException() = default;
};

/**
 * @brief Exception class for connection-related errors.
 */
class ConnectionException : public DataManagerException {
 public:
    /**
     * @brief Constructs a new ConnectionException object.
     *
     * @param message The exception message.
     */
    explicit ConnectionException(const std::string &message) :
        DataManagerException("Connection error: " + message) {}
};

/**
 * @brief Exception class for parsing-related errors.
 */
class ParseException : public DataManagerException {
 public:
    /**
     * @brief Constructs a new ParseException object.
     *
     * @param message The exception message.
     */
    explicit ParseException(const std::string &message) :
        DataManagerException("Parse error: " + message) {}
};

/**
 * @brief Exception class for timeout-related errors.
 */
class TimeoutException : public DataManagerException {
 public:
    /**
     * @brief Constructs a new TimeoutException object.
     *
     * @param message The exception message.
     */
    explicit TimeoutException(const std::string &message) :
        DataManagerException(message) {}
};

/**
 * @brief Exception class for invalid data errors.
 */
class InvalidDataException : public DataManagerException {
 public:
    /**
     * @brief Constructs a new InvalidDataException object.
     *
     * @param message The exception message.
     */
    explicit InvalidDataException(const std::string &message) :
        DataManagerException("Invalid data: " + message) {}
};

}  // namespace GUI
