#pragma once
#include <exception>
#include <string>

namespace GUI {
/**
 * @brief Base class for all graphical exceptions
 */
class GraphicalException : public std::exception {
    public:
        /**
         * @brief Constructor
         * @param message The error message
         */
        GraphicalException(const std::string &message) : _message(message) {}

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

/**
 * @brief Exception thrown when window creation fails
 */
class WindowCreationException : public GraphicalException {
    public:
        WindowCreationException(const std::string &message)
            : GraphicalException("Window creation failed: " + message) {}
};

/**
 * @brief Exception thrown when a texture loading fails
 */
class TextureLoadingException : public GraphicalException {
    public:
        TextureLoadingException(const std::string &filename)
            : GraphicalException("Failed to load texture: " + filename) {}
};

/**
 * @brief Exception thrown when a shader compilation fails
 */
class ShaderCompilationException : public GraphicalException {
    public:
        ShaderCompilationException(const std::string &message)
            : GraphicalException("Shader compilation failed: " + message) {}
};

/**
 * @brief Exception thrown when a font loading fails
 */
class FontLoadingException : public GraphicalException {
    public:
        FontLoadingException(const std::string &filename)
            : GraphicalException("Failed to load font: " + filename) {}
};

/**
 * @brief Exception thrown when a rendering operation fails
 */
class RenderingException : public GraphicalException {
    public:
        RenderingException(const std::string &message)
            : GraphicalException("Rendering error: " + message) {}
};
}  // namespace GUI
