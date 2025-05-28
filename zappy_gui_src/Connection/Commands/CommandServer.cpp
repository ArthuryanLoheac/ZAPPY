#include <string>

#include "Connection/ServerGUI.hpp"

namespace GUI {
void GUI::ServerGUI::welcomeCommand(std::vector<std::string> &args) {
    (void) args;
    sendDatasToServer("GRAPHIC\n");
}

}  // namespace GUI
