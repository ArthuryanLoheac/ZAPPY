#include "ServerGuiConnection.hpp"

namespace GUI {
void GUI::ServerGuiConnection::welcomeCommand(std::vector<std::string> &args)
{
    (void) args;
    sendDatasToServer("GRAPHIC\n");
}

} // namespace GUI