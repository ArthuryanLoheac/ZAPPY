#include "ServerGuiConnection.hpp"
#include <iostream>

namespace GUI
{
void GUI::ServerGuiConnection::handleCommand(std::string &command)
{
    std::cout << "Command received and handled : " << command << std::endl;
}
} // namespace GUI