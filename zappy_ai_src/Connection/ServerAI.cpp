#include <string.h>
#include <unistd.h>
#include <sys/poll.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdio>
#include <regex>
#include <sstream>

#include "Connection/ServerAI.hpp"
#include "DataManager/DataManager.hpp"
#include "../Logic/Core.hpp"
#include "../modules/FoodGatheringModule.hpp"
#include "../modules/CommunicationModule.hpp"

namespace AI {

ServerAI::ServerAI() {
}

// Main server loop
void ServerAI::startServer() {
    int ready = 0;

    Logic& logic = Logic::getInstance();
    logic.addModule(new FoodGatheringModule());
    logic.addModule(new CommunicationModule());

    while (DataManager::i().running) {
        std::cout << "Waiting for server messages..." << std::endl;
        
        ready = poll(&AI::ServerAI::i().fd, 1, -1);
        if (ready == -1)
            throw std::runtime_error("Poll error occurred");

        if (fd.revents & POLLIN)
            handleIncomingData();
        
        if (fd.revents & POLLOUT)
            handleOutgoingData();
            
        // Execute the highest priority module
        logic.executeHighestPriorityModule();
    }
}

// Handle incoming data
void ServerAI::handleIncomingData() {
    readDatasFromServer();
    
    std::string msg;
    while (!(msg = getNextMessage()).empty()) {
        processServerResponse(msg);
    }
}

// Handle outgoing data
void ServerAI::handleOutgoingData() {
    sendQueuedMessages();
    
    auto& cmdQueue = Logic::getInstance().getCommandQueue();
    while (!cmdQueue.empty()) {
        sendDatasToServer(cmdQueue.front() + "\n");
        cmdQueue.pop();
    }
}

// Read data from the server
void ServerAI::readDatasFromServer() {
    char bufferTemp[1024];
    ssize_t bytes_read = 0;

    bytes_read = read(server_fd, bufferTemp, sizeof(bufferTemp) - 1);
    if (bytes_read <= 0)
        throw std::runtime_error("Error reading from server");
    bufferTemp[bytes_read] = '\0';
    std::cout << "Received data: " << bufferTemp << std::endl;
    buffer.append(bufferTemp);
    handleCommand();
}

// Process server response
void ServerAI::processServerResponse(const std::string& response) {
    // Check if this is an inventory response
    if (lastCommand.find("Inventory") != std::string::npos) {
        parseInventoryResponse(response);
    }
    
    // Pass to the logic system for general handling
    Logic::getInstance().handleServerResponse(response);
}

// Parse inventory response
void ServerAI::parseInventoryResponse(const std::string& response) {
    // Expected format: [ food 345 , sibur 3 , phiras 5 , ... , deraumere 0]
    if (response.find("[") != std::string::npos && response.find("]") != std::string::npos) {
        std::map<std::string, int> items = extractInventoryItems(response);
        Logic& logic = Logic::getInstance();
        
        // Update each item in the logic inventory
        for (const auto& [item, count] : items) {
            logic.setItemQuantity(item, count);
        }
    }
}

// Extract inventory items from response string
std::map<std::string, int> ServerAI::extractInventoryItems(const std::string& inventoryStr) {
    std::map<std::string, int> items;
    
    // Remove brackets
    std::string cleaned = inventoryStr;
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '['), cleaned.end());
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), ']'), cleaned.end());
    
    // Split by commas
    std::stringstream ss(cleaned);
    std::string item;
    
    while (std::getline(ss, item, ',')) {
        // Trim whitespace
        item.erase(0, item.find_first_not_of(" \t"));
        item.erase(item.find_last_not_of(" \t") + 1);
        
        // Extract item name and count
        std::stringstream itemStream(item);
        std::string name;
        int count;
        
        if (itemStream >> name >> count) {
            items[name] = count;
        }
    }
    
    return items;
}

// Process commands from input buffer
void ServerAI::handleCommand() {
    while (buffer.find("\n") != std::string::npos) {
        size_t pos = buffer.find("\n");
        std::string command = buffer.substr(0, pos);
        buffer.erase(0, pos + 1);

        if (command.empty()) continue;
        processCommand(command);
    }
}

// Process a single command
void ServerAI::processCommand(std::string& command) {
    std::vector<std::string> args = parseCommands(command);
    if (args.empty()) return;

    std::string cmdName = args[0];
    for (size_t i = 0; i < cmdName.length(); ++i)
        cmdName[i] = toupper(cmdName[i]);
    
    executeServerCommand(cmdName, args);
}

// Execute a server command
void ServerAI::executeServerCommand(const std::string& cmdName, const std::vector<std::string>& args) {
    auto it = commands.find(cmdName);
    if (it != commands.end()) {
        try {
            if (AI::DataManager::i().getDebug())
                printf("\033[1;32m[OK]\033[0m Received Command: %s\n", cmdName.c_str());
                
            (this->*(it->second))(const_cast<std::vector<std::string>&>(args));
        } catch (const std::exception &e) {
            if (AI::DataManager::i().getErrors()) {
                printf("\033[1;31m[ERROR]\033[0m %s : ", e.what());
                for (const auto& arg : args)
                    printf(" %s", arg.c_str());
                printf("\n");
            }
        }
    } else if (waitingPos || waitingId) {
        returnWelcomeCommand(const_cast<std::vector<std::string>&>(args));
    }
}

// Message handling methods
std::string ServerAI::getNextMessage() {
    size_t pos = buffer.find('\n');
    if (pos == std::string::npos)
        return "";
    std::string message = buffer.substr(0, pos);
    buffer.erase(0, pos + 1);
    return message;
}

void ServerAI::queueMessage(const std::string &message) {
    sendQueue.push(message);
    if (AI::DataManager::i().getDebug())
        printf("[OK] Queued message: %s\n", message.c_str());
}

void ServerAI::sendQueuedMessages() {
    while (!sendQueue.empty()) {
        std::string message = sendQueue.front();
        sendQueue.pop();
        sendDatasToServer(message);
    }
}

std::vector<std::string> ServerAI::parseCommands(std::string &command) {
    std::vector<std::string> args;
    size_t pos = 0;

    while ((pos = command.find(' ')) != std::string::npos) {
        args.push_back(command.substr(0, pos));
        command.erase(0, pos + 1);
    }
    args.push_back(command);
    if (args.empty()) {
        std::cerr << "Empty command received." << std::endl;
        throw std::runtime_error("Empty command received");
    }
    return args;
}

void ServerAI::sendDatasToServer(const std::string &message) {
    if (fd.revents & POLLOUT) {
        ssize_t bytes_sent = write(server_fd, message.c_str(), message.size());
        if (bytes_sent == -1) {
            throw std::runtime_error("Error sending data to server");
        }
        if (AI::DataManager::i().getDebug())
            printf("[OK] Sent data: %s\n", message.c_str());
        lastCommand = message;
    } else {
        // If we can't send right now, re-queue the message
        queueMessage(message);
    }
}

// Welcome handshake methods
void ServerAI::welcomeCommand(std::vector<std::string> &args) {
    (void) args;
    sendDatasToServer(DataManager::i().getTeam() + "\n");
    waitingPos = true;
    waitingId = true;
}

void ServerAI::returnWelcomeCommand(std::vector<std::string> &args) {
    try {
        if (waitingPos && args.size() == 2) {
            processWelcomePosition(args);
        }
        
        if (waitingId && args.size() == 1) {
            processWelcomeId(args);
        }
    } catch (const std::exception &e) {
        std::cerr << "Error Welcome: " << e.what() << std::endl;
    }
}

void ServerAI::processWelcomePosition(const std::vector<std::string> &args) {
    int xMap = std::stoi(args[0]);
    int yMap = std::stoi(args[1]);
    if (xMap < 0 || yMap < 0)
        throw std::runtime_error("Invalid position coord");
    DataManager::i().xMap = xMap;
    DataManager::i().yMap = yMap;
    waitingPos = false;
}

void ServerAI::processWelcomeId(const std::vector<std::string> &args) {
    int id = std::stoi(args[0]);
    if (id < 0)
        throw std::runtime_error("Invalid ID");
    DataManager::i().id = id;
    waitingId = false;
}

}  // namespace AI
