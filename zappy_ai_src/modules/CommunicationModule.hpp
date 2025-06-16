/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/


#ifndef COMMUNICATIONMODULE_HPP
#define COMMUNICATIONMODULE_HPP

#include "AIModule.hpp"

class CommunicationModule : public AIModule {
public:
    CommunicationModule();
    void execute() override;
    float getPriority() override;
};

#endif // COMMUNICATIONMODULE_HPP