/*
** EPITECH PROJECT, 2024
** ZAPPY
** File description:
** GUI Exceptions
*/

#ifndef GUI_EXCEPTIONS_HPP_
#define GUI_EXCEPTIONS_HPP_

#include <exception>
#include <string>

namespace GUI {

class GuiException : public std::exception {
    public:
        GuiException(const std::string& message) : _message(message) {}
        virtual ~GuiException() = default;

        const char* what() const noexcept override {
            return _message.c_str();
        }

    protected:
        std::string _message;
};
} // namespace gui

#endif /* !GUI_EXCEPTIONS_HPP_ */