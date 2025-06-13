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
#include "../modules/AIModule.hpp"
#include "AIBase.hpp"

class PrioritySystem : public AIBase {
public:
    void addModule(AIModule* module) {
        modules.push_back(module);
    }

    void executeHighestPriorityModule() {
        if (modules.empty()) return;

        auto highestPriorityModule = std::max_element(modules.begin(), modules.end(),
            [](AIModule* a, AIModule* b) {
                return a->getPriority() < b->getPriority();
            });

        GameData data;
        std::cout << "(EMPTY) Executing highest priority module with priority: "
                  << (*highestPriorityModule)->getPriority() << std::endl;
        std::string commands = (*highestPriorityModule)->execute(data);
        size_t start = 0, end;
        while ((end = commands.find('\n', start)) != std::string::npos) {
            std::string cmd = commands.substr(start, end - start);
            if (!cmd.empty())
                commandQueue.push(cmd);
            start = end + 1;
        }
        std::string lastCmd = commands.substr(start);
        if (!lastCmd.empty())
            commandQueue.push(lastCmd);
    }

    void sendCommand(const std::string& command) {
        commandQueue.push(command);
    }
    void handleServerResponse(const std::string& response) {
        if (commandQueue.empty()) {
            std::cerr << "No command in queue to handle response." << std::endl;
            return;
        }

        std::string originalCommand = commandQueue.front();
        commandQueue.pop();

        std::string messageType = extractMessageType(originalCommand, response);
        notify(messageType, response);
    }
    std::queue<std::string>& getCommandQueue() { return commandQueue; }


private:
    std::vector<AIModule*> modules;
    std::queue<std::string> commandQueue;
    std::string extractMessageType(const std::string& command, const std::string& response) {
        if (command.find("Inventory") == 0)
            return "Inventory" + response;
        if (command.find("Look") == 0)
            return "Look" + response;
        if (command.find("Take") == 0)
            return "Take" + response;
        if (command.find("Set") == 0)
            return "Set" + response;
        if (command.find("Broadcast") == 0)
            return "Broadcast" + response;
        if (command.find("Forward") == 0)
            return "Forward" + response;
        if (command.find("Right") == 0)
            return "Right" + response;
        if (command.find("Left") == 0)
            return "Left" + response;
        if (command.find("Eject") == 0)
            return "Eject" + response;
        if (command.find("Incantation") == 0)
            return "Incantation" + response;
        // ...add more as needed...
        return "Unknown" + response;
    }
};

#endif // PRIORITYSYSTEM_HPP