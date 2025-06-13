#include <string.h>
#include <unistd.h>
#include <sys/poll.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cstdio>

#include "Connection/ServerAI.hpp"
#include "DataManager/DataManager.hpp"
#include "../Logic/Core.hpp"
#include "../modules/FoodGatheringModule.hpp"
#include "../modules/CommunicationModule.hpp"

namespace AI {

ServerAI::ServerAI() {
}

void AI::ServerAI::handleCommand() {
    while (buffer.find("\n") != std::string::npos) {
        size_t pos = buffer.find("\n");
        std::string command = buffer.substr(0, pos);
        buffer.erase(0, pos + 1);

        if (command.empty()) continue;

        std::vector<std::string> args = parseCommands(command);
        if (args.empty()) continue;

        for (size_t i = 0; i < args[0].length(); ++i)
            args[0][i] = toupper(args[0][i]);

        auto it = commands.find(args[0]);
        if (it != commands.end()) {
            try {
                if (AI::DataManager::i().getDebug())
                    printf("\033[1;32m[OK]\033[0m Received Command: %s\n",
                        args[0].c_str());
                (AI::ServerAI::i().*(it->second))(args);
            } catch (const std::exception &e) {
                if (AI::DataManager::i().getErrors()) {
                    printf("\033[1;31m[ERROR]\033[0m %s : ", e.what());
                    for (size_t i = 0; i < args.size(); i++)
                        printf(" %s", args[i].c_str());
                    printf("\n");
                }
            }
        } else if (waitingPos || waitingId) {
            returnWelcomeCommand(args);
        }
    }
}

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

        if (fd.revents & POLLIN) {
            readDatasFromServer();

            std::string msg;
            while (!(msg = getNextMessage()).empty()) {
                logic.handleServerResponse(msg);
            }
        }
        
        // Execute the highest priority module
        logic.executeHighestPriorityModule();
        
        // Send all queued commands to the server
        if (fd.revents & POLLOUT) {
            sendQueuedMessages();
            
            auto& cmdQueue = logic.getCommandQueue();
            while (!cmdQueue.empty()) {
                sendDatasToServer(cmdQueue.front() + "\n");
                cmdQueue.pop();
            }
        }
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
        if (args.size() == 2 && waitingPos) {
            int xMap = std::stoi(args[0]);
            int yMap = std::stoi(args[1]);
            if (xMap < 0 || yMap < 0)
                throw std::runtime_error("Invalid position coord");
            DataManager::i().xMap = xMap;
            DataManager::i().yMap = yMap;
            waitingPos = false;
        }
        if (args.size() == 1 && waitingId) {    
            int id = std::stoi(args[0]);
            if (id < 0)
                throw std::runtime_error("Invalid ID");
            DataManager::i().id = id;
            waitingId = false;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error Welcome: " << e.what() << std::endl;
    }
}

}  // namespace AI
