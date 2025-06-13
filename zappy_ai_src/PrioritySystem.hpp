/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/


#ifndef PRIORITYSYSTEM_HPP
#define PRIORITYSYSTEM_HPP

#include <vector>
#include <algorithm>
#include <queue>
#include <string>
#include <iostream>
#include "../modules/AIModule.hpp"
#include "Logic/AIBase.hpp"

class PrioritySystem : public AIBase {
public:
    void addModule(AIModule* module) {
        modules.push_back(module);
    }

    void executeHighestPriorityModule() {
        AIModule* highest = nullptr;
        float maxPriority = -1.0f;
        for (auto* mod : modules) {
            float prio = mod->getPriority();
            if (prio > maxPriority) {
                maxPriority = prio;
                highest = mod;
            }
        }
        std::cout << "execute here the highest priority module with priority: " << maxPriority << std::endl;
    }

    // Send a command to the server and queue it for response matching
    void sendCommand(const std::string& command) {
        // ...send command to server (implementation-dependent)...
        commandQueue.push(command);
    }

    // Call this when a response is received from the server
    void handleServerResponse(const std::string& response) {
        if (commandQueue.empty())
            return;

        std::string originalCommand = commandQueue.front();
        commandQueue.pop();

        std::string messageType = extractMessageType(originalCommand, response);
        notify(messageType, response);
    }

private:
    std::vector<AIModule*> modules;
    std::queue<std::string> commandQueue;

    std::string extractMessageType(const std::string& command, const std::string& response) {

        if (command.find("Inventory") == 0)
            return "Inventory";
        if (command.find("Look") == 0)
            return "Look";
        if (command.find("Take") == 0)
            return "Take";
        if (command.find("Set") == 0)
            return "Set";
        if (command.find("Broadcast") == 0)
            return "Broadcast";
        if (command.find("Forward") == 0)
            return "Forward";
        if (command.find("Right") == 0)
            return "Right";
        if (command.find("Left") == 0)
            return "Left";
        if (command.find("Eject") == 0)
            return "Eject";
        if (command.find("Incantation") == 0)
            return "Incantation";
        return "Unknown";
    }
};

#endif // PRIORITYSYSTEM_HPP