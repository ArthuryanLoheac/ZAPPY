#include <iostream>
#include <string>
#include <vector>

#include "Connection/ServerAI.hpp"
#include "Exceptions/ServerAIExceptions.hpp"
#include "DataManager/DataManager.hpp"
#include "ServerAI.hpp"

void printError(const std::exception &e, std::vector<std::string> &args) {
    std::cerr << "Error: " << e.what() << std::endl;
    std::cerr << "Args: ";
    for (const auto &arg : args)
        std::cerr << arg << " ";
    std::cerr << std::endl;
}

namespace AI {


void ServerAI::welcomeCommand(std::vector<std::string> &args) {
    (void) args;
    sendDatasToServer(DataManager::i().getTeam() + "\n");
    waitingPos = true;
    waitingId = true;
}

void ServerAI::returnWelcomeCommand(std::vector<std::string> &args)
{
    if (args.size() == 2 && waitingPos) {
        int xMap = std::stoi(args[0]);
        int yMap = std::stoi(args[1]);
        if (xMap < 0 || yMap < 0)
            throw AI::CommandParsingException("Invalid position coordinates");
        DataManager::i().xMap = xMap;
        DataManager::i().yMap = yMap;
        waitingPos = false;
    }
    if (args.size() == 1 && waitingId) {
        int id = std::stoi(args[0]);
        if (id < 0)
            throw AI::CommandParsingException("Invalid ID");
        DataManager::i().id = id;
        waitingId = false;
    }
    if (waitingPos == false && waitingId == false) {
        sendDatasToServer("Forward\n");
    }
}

void ServerAI::takeFoodCommand(std::vector<std::string> &args)
{
    (void) args;
    sendDatasToServer("Forward\n");
}

void ServerAI::forwardCommand(std::vector<std::string> &args)
{
    (void) args;
    sendDatasToServer("Take food\n");
}

}  // namespace AI
