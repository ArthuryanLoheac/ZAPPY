#include "Logic/Core.hpp"

#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <string>
#include <typeinfo>
#include <utility>
#include <vector>

#include "../Logic/AIBase.hpp"
#include "../Interface/Interface.hpp"
#include "Logic/PrioritySystem.hpp"
#include "../modules/FoodGatheringModule.hpp"
#include "../modules/KirbyModule.hpp"
#include "../modules/RoleAttributionModule.hpp"
#include "../modules/ElevationModule.hpp"
#include "../modules/DisruptionModule.hpp"
#include "../modules/RessourceGatheringSpawning.hpp"
#include "../modules/FeederModule.hpp"
#include "../../libc/include/logs.h"

void CommandHistory::addCommandResponse(const std::string& command,
                                       const std::string& response) {
    history.emplace_back(command, response);
    if (history.size() > MAX_HISTORY_SIZE) {
        history.erase(history.begin());
    }
}

const std::vector<std::pair<std::string, std::string>>&
CommandHistory::getHistory() const {
    return history;
}

std::pair<std::string, std::string> CommandHistory::getLastCommandResponse()
    const {
    if (history.empty()) {
        return {"", ""};
    }
    return history.back();
}

Logic::Logic() : level(1), roleModulesSetup(false) {
    inventory["Food"] = 10;
}

void Logic::addCommandResponse(const std::string& command,
                              const std::string& response) {
    commandHistory.addCommandResponse(command, response);
}

void Logic::addModule(std::unique_ptr<AIModule> module) {
    modules.push_back(std::move(module));
}

void Logic::executeHighestPriorityModule() {
    if (modules.empty()) return;
    static int inventoryCheckCounter = 0;
    if (++inventoryCheckCounter >= 10) {
        inventoryCheckCounter = 0;
        AI::Interface::i().sendCommand(INVENTORY);
        return;
    }

    auto highestPriorityModuleIt = std::max_element(
        modules.begin(), modules.end(),
        [](const std::unique_ptr<AIModule>& a,
            const std::unique_ptr<AIModule>& b) {
            return a->getPriority() > b->getPriority();
        });
    const auto& module = *highestPriorityModuleIt;
    LOG_INFO("Executing module: %s with priority: %.2f", 
             typeid(*module).name(), module->getPriority());
    module->execute();
}

void Logic::queueCommand(const std::string& command) {
    LOG_INFO("Queuing command: %s", command.c_str());
    commandQueue.push(command);
}

std::queue<std::string>& Logic::getCommandQueue() {
    return commandQueue;
}

void Logic::handleServerResponse(const std::string& response) {
    if (commandQueue.empty()) {
        LOG_ERROR("No command in queue to handle response.");
        return;
    }

    std::string originalCommand = commandQueue.front();
    commandQueue.pop();

    addCommandResponse(originalCommand, response);
}

std::string Logic::extractMessageType(const std::string& command,
                                     const std::string& response) {
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

const std::vector<std::pair<std::string, std::string>>&
Logic::getCommandHistory() const {
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

void Logic::setLevel(int16_t newLevel) {
    level = newLevel;
}

int16_t Logic::getLevel() const {
    return level;
}

/**
 * @brief Get the RoleAttributionModule instance
 * @return Pointer to the RoleAttributionModule, or nullptr if not found
 */
RoleAttributionModule* Logic::getRoleModule() {
    for (auto& module : modules) {
        if (auto roleModule =
            dynamic_cast<RoleAttributionModule*>(module.get())) {
            return roleModule;
        }
    }
    return nullptr;
}

/**
 * @brief Setup modules based on the assigned role
 */
void Logic::setupRoleBasedModules() {
    auto roleModule = getRoleModule();
    if (!roleModule || !roleModule->isRoleAssigned()) {
        return;
    }

    Role currentRole = roleModule->getCurrentRole();

    switch (currentRole) {
        case Role::HARVESTER:
            LOG_INFO("Setting up modules for role: HARVESTER");
            addModule(std::make_unique<KirbyModule>());
            break;

        case Role::LEVELER:
            LOG_INFO("Setting up modules for role: LEVELER");
            addModule(std::make_unique<DisruptionModule>());
            addModule(std::make_unique<RessourceGatheringSpawning>());
            addModule(std::make_unique<ElevationModule>());
            break;

        case Role::DISRUPTER:
            LOG_INFO("Setting up modules for role: DISRUPTER");
            addModule(std::make_unique<DisruptionModule>());
            break;

        case Role::FEEDER:
            LOG_INFO("Setting up modules for role: FEEDER");
            addModule(std::make_unique<FeederModule>());
            break;

        default:
            LOG_INFO("Setting up modules for role: UNKNOWN (no additional modules added)");
            return;
    }

    roleModulesSetup = true;
}

/**
 * @brief Check if role-based modules have been set up
 * @return True if modules have been set up
 */
bool Logic::hasRoleBasedModulesSetup() const {
    return roleModulesSetup;
}
