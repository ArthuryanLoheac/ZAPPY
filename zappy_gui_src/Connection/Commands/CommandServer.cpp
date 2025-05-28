#include <string>

#include "Connection/ServerGUI.hpp"
#include "Exceptions/ServerGUIExceptions.hpp"
#include <GameDataManager.hpp>

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
}
} // namespace GUI
