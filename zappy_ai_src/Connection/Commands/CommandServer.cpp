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
    sendDatasToServer("Look\n");
}

void ServerAI::LookCommand(std::vector<std::string> &args)
{
    std::vector<std::vector<std::string>> looks = getLook(args);

    int i = 0;

    for (const auto &look : looks) {
        for (const auto &item : look) {
            if (!item.empty() && item == "food") {
                computePathTo(i, "Take food\n");
                break;
            }
        }
        i++;
    }
    if (!executeNextPathCommand())
        sendDatasToServer("Forward\n");
}

void ServerAI::computePathTo(int i, std::string lastCommand)
{
    if (i == -1)
        return;
    if (i == 1) {
        path.push_back("Forward\n");
        path.push_back("Right\n");
        path.push_back("Forward\n");
    } else if (i == 2) {
        path.push_back("Forward\n");
    } else if (i == 3) {
        path.push_back("Forward\n");
        path.push_back("Left\n");
        path.push_back("Forward\n");
    } else if (i == 4) {
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Right\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
    } else if (i == 5) {
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Right\n");
        path.push_back("Forward\n");
    } else if (i == 6) {
        path.push_back("Forward\n");
        path.push_back("Forward\n");
    } else if (i == 7) {
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Left\n");
        path.push_back("Forward\n");
    } else if (i == 8) {
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Left\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
    } else if (i == 9) {
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Right\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
    } else if (i == 10) {
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Right\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
    } else if (i == 11) {
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Right\n");
        path.push_back("Forward\n");
    } else if (i == 12) {
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
    } else if (i == 13) {
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Left\n");
        path.push_back("Forward\n");
    } else if (i == 14) {
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Left\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
    } else if (i == 15) {
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Left\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
        path.push_back("Forward\n");
    }
    if (!lastCommand.empty() && lastCommand != "Look\n")
        path.push_back(lastCommand);
}

bool ServerAI::executeNextPathCommand() {
    if (path.empty())
        return false;
    sendDatasToServer(path.front());
    path.erase(path.begin());
    return true;
}

void ServerAI::depileOrLookCommand(std::vector<std::string> &args)
{
    (void) args;
    if (!executeNextPathCommand())
        sendDatasToServer("Look\n");
}

std::vector<std::vector<std::string>> ServerAI::getLook(std::vector<std::string> &args)
{
    std::string all;
    std::vector<std::string> looks;
    std::string delimiter = ",";
    std::vector<std::vector<std::string>> looksItem;
    std::string delimiterItem = " ";
    size_t pos = 0;

    for (const auto &arg : args)
        all += arg + " ";
    for (size_t i = 0; i < all.size(); ++i) {
        if (all[i] == '[' || all[i] == ']')
            all[i] = ' ';
    }
    while ((pos = all.find(delimiter)) != std::string::npos) {
        looks.push_back(all.substr(0, pos));
        all.erase(0, pos + delimiter.length());
    }
    if (!all.empty())
        looks.push_back(all);
    for (const auto &look : looks) {
        std::vector<std::string> items;
        size_t posItem = 0;
        std::string lookItem = look;
        while ((posItem = lookItem.find(delimiterItem)) != std::string::npos) {
            items.push_back(lookItem.substr(0, posItem));
            lookItem.erase(0, posItem + delimiterItem.length());
        }
        if (!lookItem.empty())
            items.push_back(lookItem);
        looksItem.push_back(items);
    }
    return looksItem;
}

}  // namespace AI
