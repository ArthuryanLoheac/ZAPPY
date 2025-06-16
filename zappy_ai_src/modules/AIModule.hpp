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

    // Execute the behavior of the module - no parameters, no return value
    virtual void execute() = 0;

    // Get the priority of the module
    virtual float getPriority() = 0;
};

#endif // AIMODULE_HPP