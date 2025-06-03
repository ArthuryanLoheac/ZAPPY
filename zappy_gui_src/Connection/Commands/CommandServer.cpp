#include <string>

#include "Connection/ServerGUI.hpp"
#include "Exceptions/ServerGUIExceptions.hpp"
#include "Window/window.hpp"
#include "DataManager/GameDataManager.hpp"
#include "DataManager/DataManager.hpp"

namespace GUI {
void GUI::ServerGUI::welcomeCommand(std::vector<std::string> &args) {
    (void) args;
    sendDatasToServer("GRAPHIC\n");
}

void ServerGUI::mszCommand(std::vector<std::string> &args) {
    if (args.size() != 3)
        throw GUI::CommandParsingException("Invalid msz command format");
    int width = std::stoi(args[1]);
    int height = std::stoi(args[2]);
    if (width <= 0 || height <= 0)
        throw GUI::CommandParsingException("Invalid dimensions in msz command");
    GUI::GameDataManager::i().setWidth(width);
    GUI::GameDataManager::i().setHeight(height);

    GUI::Window::i().setupWorld();
}

void GUI::ServerGUI::enwCommand(std::vector<std::string> &args) {
    if (args.size() != 5)
        throw GUI::CommandParsingException("Invalid enw command format");
    if (args[1].size() < 2 || args[2].size() < 2)
        throw GUI::CommandParsingException("Invalid team or id name");
    try {
        int id = std::stoi(args[1].substr(1));
        int team = std::stoi(args[2].substr(1));
        int x = std::stoi(args[3]);
        int y = std::stoi(args[4]);

        GUI::GameDataManager::i().getTile(x, y).addEgg(id, team);
    } catch (const std::exception &e) {
        throw GUI::CommandParsingException("Invalid parameters in enw command");
    }
}

void ServerGUI::tnaCommand(std::vector<std::string> &args) {
    if (args.size() != 2)
        throw GUI::CommandParsingException("Invalid tna command format");

    std::string teamName = args[1];
    GUI::GameDataManager::i().addTeam(teamName);
}

void ServerGUI::bctCommand(std::vector<std::string> &args) {
    if (args.size() != 10)
        throw GUI::CommandParsingException("Invalid bct command format");
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

        GUI::GameDataManager::i().getTile(x, y).setRessources(food, r1, r2, r3,
            r4, r5, r6);
    } catch (const std::exception &e) {
        throw GUI::CommandParsingException("Invalid parameters in bct command");
    }
}

void ServerGUI::sgtCommand(std::vector<std::string> &args) {
    if (args.size() != 2)
        throw GUI::CommandParsingException("Invalid sgt command format");
    try {
        int f = std::stoi(args[1]);

        DataManager::i().setFrequency(f);
    } catch (const std::exception &e) {
        throw GUI::CommandParsingException("Invalid parameters in sgt command");
    }
}

}  // namespace GUI
