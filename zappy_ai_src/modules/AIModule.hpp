/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/


#ifndef AIMODULE_HPP
#define AIMODULE_HPP

#include <string>

class AIModule {
public:
    virtual ~AIModule() {}

    // Execute the behavior of the module and return commands as a string (commands separated by newlines)
    virtual std::string execute() = 0;

    // Get the priority of the module
    virtual float getPriority() const = 0;
};

#endif // AIMODULE_HPP