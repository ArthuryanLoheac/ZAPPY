#pragma once

#include <exception>
#include <string>

namespace GUI {

/**
 * @brief Base class for all SoundsManager exceptions
 */
class SoundsManagerException : public std::exception {
 public:
    explicit SoundsManagerException(const std::string& message)
        : _message("SoundsManager error: " + message) {}
    virtual ~SoundsManagerException() = default;

    const char* what() const noexcept override {
        return _message.c_str();
    }

 protected:
    std::string _message;
};

/**
 * @brief Exception thrown when a music file fails to load
 */
class MusicLoadingException : public SoundsManagerException {
 public:
    explicit MusicLoadingException(const std::string& path)
        : SoundsManagerException("Failed to load music file: " + path) {}
};

/**
 * @brief Exception thrown when a sound file fails to load
 */
class SoundLoadingException : public SoundsManagerException {
 public:
    explicit SoundLoadingException(const std::string& path)
        : SoundsManagerException("Failed to load sound file: " + path) {}
};

}  // namespace GUI
