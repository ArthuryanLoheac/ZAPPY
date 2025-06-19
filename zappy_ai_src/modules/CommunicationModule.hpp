/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/

#ifndef ZAPPY_AI_SRC_MODULES_COMMUNICATIONMODULE_HPP_
#define ZAPPY_AI_SRC_MODULES_COMMUNICATIONMODULE_HPP_

#include "modules/AIModule.hpp"

class CommunicationModule : public AIModule {
 public:
    CommunicationModule();
    void execute() override;
    float getPriority() override;
 private:
    int id = -1;
};

#endif  // ZAPPY_AI_SRC_MODULES_COMMUNICATIONMODULE_HPP_
