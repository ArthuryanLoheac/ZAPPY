#include "Core.hpp"
#include <iostream>
#include "PrioritySystem.hpp"
#include "../modules/FoodGatheringModule.hpp"
#include "../modules/CommunicationModule.hpp"
#include "../Connection/ServerAI.hpp"

// CommandHistory implementation
void CommandHistory::addCommandResponse(const std::string& command, const std::string& response) {
    history.emplace_back(command, response);
    if (history.size() > MAX_HISTORY_SIZE) {
        history.erase(history.begin());
    }
}

const std::vector<std::pair<std::string, std::string>>& CommandHistory::getHistory() const {
    return history;
}

std::pair<std::string, std::string> CommandHistory::getLastCommandResponse() const {
    if (history.empty()) {
        return {"", ""};
    }
    return history.back();
}

Logic::Logic() : level(1) {
    inventory["Food"] = 10;
}

void Logic::addCommandResponse(const std::string& command, const std::string& response) {
    commandHistory.addCommandResponse(command, response);
}

void Logic::addModule(AIModule* module) {
    modules.push_back(module);
}

void Logic::executeHighestPriorityModule() {
    if (modules.empty()) return;

    auto highestPriorityModule = std::max_element(modules.begin(), modules.end(),
        [](AIModule* a, AIModule* b) {
            return a->getPriority() < b->getPriority();
        });

    std::string commands = (*highestPriorityModule)->execute();
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

void Logic::queueCommand(const std::string& command) {
    commandQueue.push(command);
}

std::queue<std::string>& Logic::getCommandQueue() {
    return commandQueue;
}

void Logic::handleServerResponse(const std::string& response) {
    if (commandQueue.empty()) {
        std::cerr << "No command in queue to handle response." << std::endl;
        return;
    }

    std::string originalCommand = commandQueue.front();
    commandQueue.pop();

    // Store the command-response pair in history
    addCommandResponse(originalCommand, response);
}

std::string Logic::extractMessageType(const std::string& command, const std::string& response) {
    (void)response;
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
