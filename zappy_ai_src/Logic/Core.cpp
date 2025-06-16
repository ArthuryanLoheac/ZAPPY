#include "Core.hpp"
#include <iostream>
#include "PrioritySystem.hpp"
#include "../modules/FoodGatheringModule.hpp"
#include "../modules/CommunicationModule.hpp"

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

void Logic::addModule(std::unique_ptr<AIModule> module) {
    modules.push_back(std::move(module));
}

void Logic::executeHighestPriorityModule() {
    if (modules.empty()) return;

    auto highestPriorityModuleIt = std::max_element(modules.begin(), modules.end(),
        [](const std::unique_ptr<AIModule>& a, const std::unique_ptr<AIModule>& b) {
            return a->getPriority() > b->getPriority();
        });
    (*highestPriorityModuleIt)->execute();
}

void Logic::queueCommand(const std::string& command) {
    std::cout << "Queuing command: " << command << std::endl;
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

const std::vector<std::pair<std::string, std::string>>& Logic::getCommandHistory() const {
    return commandHistory.getHistory();
}

std::pair<std::string, std::string> Logic::getLastCommandResponse() const {
    return commandHistory.getLastCommandResponse();
}

void Logic::setItemQuantity(const std::string& item, int quantity) {
    inventory[item] = quantity;
}

int Logic::getItemQuantity(const std::string& item) const {
    auto it = inventory.find(item);
    if (it != inventory.end()) {
        return it->second;
    }
    return 0;
}

const std::map<std::string, int>& Logic::getInventory() const {
    return inventory;
}

void Logic::setLevel(short newLevel) {
    level = newLevel;
}

short Logic::getLevel() const {
    return level;
}
