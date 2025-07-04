/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/

#pragma once

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <queue>
#include <memory>
#include <cstdint>
#include "../modules/AIModule.hpp"
#include "../modules/RoleAttributionModule.hpp"

class CommandHistory {
 public:
    CommandHistory() = default;
    void addCommandResponse(const std::string& command,
        const std::string& response);
    const std::vector<std::pair<std::string, std::string>>& getHistory() const;
    std::pair<std::string, std::string> getLastCommandResponse() const;

 private:
    std::vector<std::pair<std::string, std::string>> history;
    static const size_t MAX_HISTORY_SIZE = 20;
};

class Logic {
 public:
    Logic();
    static Logic& getInstance() {
        static Logic instance;
        return instance;
    }

    void addModule(std::unique_ptr<AIModule> module);
    void executeHighestPriorityModule();
    void queueCommand(const std::string& command);
    std::queue<std::string>& getCommandQueue();
    void handleServerResponse(const std::string& response);
    void addCommandResponse(const std::string& command, const
        std::string& response);
    const std::vector<std::pair<std::string, std::string>>&
        getCommandHistory() const;
    std::pair<std::string, std::string> getLastCommandResponse() const;
    void setItemQuantity(const std::string& item, int quantity);
    int getItemQuantity(const std::string& item) const;
    const std::map<std::string, int>& getInventory() const;
    void setLevel(int16_t newLevel);
    int16_t getLevel() const;
    RoleAttributionModule* getRoleModule();
    void setupRoleBasedModules();
    bool hasRoleBasedModulesSetup() const;

 private:
    std::vector<std::unique_ptr<AIModule>> modules;
    std::queue<std::string> commandQueue;
    CommandHistory commandHistory;
    std::map<std::string, int> inventory;
    int16_t level;
    bool roleModulesSetup;
    std::string extractMessageType(const std::string& command,
        const std::string& response);
};

void run_ai_logic_loop();
