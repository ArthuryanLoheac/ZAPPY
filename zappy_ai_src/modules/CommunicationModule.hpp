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
    std::string execute(const GameData& data) override;
    float getPriority() const override;
};

#endif // COMMUNICATIONMODULE_HPP