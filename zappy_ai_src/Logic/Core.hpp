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
#include "../modules/AIModule.hpp"

class CommandHistory {
public:
    CommandHistory() = default;
    
    void addCommandResponse(const std::string& command, const std::string& response);
    
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
    
    // Module management
    void addModule(AIModule* module);
    void executeHighestPriorityModule();
    
    // Command queue management
    void queueCommand(const std::string& command);
    std::queue<std::string>& getCommandQueue();
    
    // Response handling
    void handleServerResponse(const std::string& response);
    
    // Command history methods
    void addCommandResponse(const std::string& command, const std::string& response);
    const std::vector<std::pair<std::string, std::string>>& getCommandHistory() const;
    std::pair<std::string, std::string> getLastCommandResponse() const;
    
    // Inventory methods
    void setItemQuantity(const std::string& item, int quantity);
    int getItemQuantity(const std::string& item) const;
    const std::map<std::string, int>& getInventory() const;
    
    // Level methods
    void setLevel(short newLevel);
    short getLevel() const;
    
private:
    std::vector<AIModule*> modules;
    std::queue<std::string> commandQueue;
    CommandHistory commandHistory;
    std::map<std::string, int> inventory;
    short level;
    
    // Helper for extracting message type from commands
    std::string extractMessageType(const std::string& command, const std::string& response);
};

void run_ai_logic_loop();
