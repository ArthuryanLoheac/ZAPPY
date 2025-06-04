#include <string>

#include "Connection/ServerGUI.hpp"
#include "Exceptions/ServerGUIExceptions.hpp"
#include "Window/window.hpp"
#include "DataManager/GameDataManager.hpp"
#include "DataManager/DataManager.hpp"
#include "ServerGUI.hpp"
#include <iostream>

namespace GUI {
void GUI::ServerGUI::welcomeCommand(std::vector<std::string> &args) {
    (void) args;
    sendDatasToServer("GRAPHIC\n");
}

void ServerGUI::mszCommand(std::vector<std::string> &args) {
    if (args.size() != 3)
        throw CommandParsingException("Invalid msz command format");
    int width = std::stoi(args[1]);
    int height = std::stoi(args[2]);
    if (width <= 0 || height <= 0)
        throw CommandParsingException("Invalid dimensions in msz command");
    GameDataManager::i().setWidth(width);
    GameDataManager::i().setHeight(height);

    GUI::Window::i().setupWorld();
}

void GUI::ServerGUI::enwCommand(std::vector<std::string> &args) {
    if (args.size() != 5)
        throw CommandParsingException("Invalid enw command format");
    if (args[1].size() < 2 || args[2].size() < 2)
        throw CommandParsingException("Invalid team or id name");
    try {
        int id = std::stoi(args[1].substr(1));
        int team = std::stoi(args[2].substr(1));
        int x = std::stoi(args[3]);
        int y = std::stoi(args[4]);

        GameDataManager::i().addEgg(id, team, x, y);
    } catch (const std::exception &e) {
        std::cerr << "Error parsing pnw command: " << e.what() << std::endl;
    }
}

void ServerGUI::tnaCommand(std::vector<std::string> &args) {
    if (args.size() != 2)
        throw CommandParsingException("Invalid tna command format");

    std::string teamName = args[1];
    GameDataManager::i().addTeam(teamName);
}

void ServerGUI::bctCommand(std::vector<std::string> &args) {
    if (args.size() != 10)
        throw CommandParsingException("Invalid bct command format");
    try {
        int x = std::stoi(args[1]);
        int y = std::stoi(args[2]);
        int food = std::stoi(args[3]);
        int r1 = std::stoi(args[4]);
        int r2 = std::stoi(args[5]);
        int r3 = std::stoi(args[6]);
        int r4 = std::stoi(args[7]);
        int r5 = std::stoi(args[8]);
        int r6 = std::stoi(args[9]);

        GameDataManager::i().getTile(x, y).setRessources(food, r1, r2, r3,
            r4, r5, r6);
    } catch (const std::exception &e) {
        std::cerr << "Error parsing pnw command: " << e.what() << std::endl;
    }
}

void ServerGUI::sgtCommand(std::vector<std::string> &args) {
    if (args.size() != 2)
        throw CommandParsingException("Invalid sgt command format");
    try {
        int f = std::stoi(args[1]);

        DataManager::i().setFrequency(f);
    } catch (const std::exception &e) {
        std::cerr << "Error parsing pnw command: " << e.what() << std::endl;
    }
}

void ServerGUI::eboCommand(std::vector<std::string> &args) {
    if (args.size() != 2)
        throw CommandParsingException("Invalid ebo command format");
    if (args[1].size() < 2)
        throw CommandParsingException("Invalid id name");
    try {
        int id = std::stoi(args[1].substr(1));

        GameDataManager::i().removeEgg(id);
    } catch (const std::exception &e) {
        std::cerr << "Error parsing pnw command: " << e.what() << std::endl;
    }
}

void ServerGUI::ediCommand(std::vector<std::string> &args) {
    if (args.size() != 2)
        throw CommandParsingException("Invalid edi command format");
    if (args[1].size() < 2)
        throw CommandParsingException("Invalid id name");
    try {
        int id = std::stoi(args[1].substr(1));

        GameDataManager::i().removeEgg(id);
    } catch (const std::exception &e) {
        std::cerr << "Error parsing pnw command: " << e.what() << std::endl;
    }
}

void ServerGUI::pnwCommand(std::vector<std::string> &args) {
    if (args.size() != 7)
        throw CommandParsingException("Invalid pnw command format");
    if (args[1].size() < 2)
        throw CommandParsingException("Invalid id name");
    try {
        int id = std::stoi(args[1].substr(1));
        int x = std::stoi(args[2]);
        int y = std::stoi(args[3]);
        int orient = std::stoi(args[4]) - 1;
        int level = std::stoi(args[5]);
        std::string teamName = args[6];
        if (orient < 0 || orient > 3)
            throw CommandParsingException("Invalid orientation in pnw command");
        if (level < 1 || level > 8)
            throw CommandParsingException("Invalid level in pnw command");

        Player::Orientation pOrient = static_cast<Player::Orientation>(orient);

        GameDataManager::i().addPlayer(id, x, y, pOrient, level, teamName);
    } catch (const std::exception &e) {
        std::cerr << "Error parsing pnw command: " << e.what() << std::endl;
    }
}
} // namespace GUI
