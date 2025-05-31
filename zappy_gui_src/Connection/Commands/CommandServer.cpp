#include <string>

#include "Connection/ServerGUI.hpp"
#include "Exceptions/ServerGUIExceptions.hpp"
#include "Window/window.hpp"
#include "DataManager/GameDataManager.hpp"

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
        int id = std::stoi(args[1].c_str() + 1);
        int team = std::stoi(args[2].c_str() + 1);
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

}  // namespace GUI
