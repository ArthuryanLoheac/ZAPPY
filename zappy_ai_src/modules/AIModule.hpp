/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/


#ifndef ZAPPY_AI_SRC_MODULES_AIMODULE_HPP_
#define ZAPPY_AI_SRC_MODULES_AIMODULE_HPP_

#include <string>

class AIModule {
 public:
    virtual ~AIModule() {}

    // Execute the behavior of the module - no parameters, no return value
    virtual void execute() = 0;

    // Get the priority of the module
    virtual float getPriority() = 0;
};

#endif  // ZAPPY_AI_SRC_MODULES_AIMODULE_HPP_
