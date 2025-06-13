/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/


#ifndef AIMODULE_HPP
#define AIMODULE_HPP

#include <string>
#include <map>
#include <vector>
#include <utility>

struct GameData {
    short level;
    std::map<std::string, int> inventory;
    std::vector<std::pair<std::string, std::string>> commandHistory;
};

class AIModule {
public:
    virtual ~AIModule() {}

    // Execute the behavior of the module with access to game data
    virtual std::string execute(const GameData& data) = 0;

    // Get the priority of the module
    virtual float getPriority() const = 0;
};

#endif // AIMODULE_HPP