#include "ServerGuiConnection.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/poll.h>
#include <string.h>

namespace GUI
{
ServerGuiConnection::ServerGuiConnection()
{
}

void GUI::ServerGuiConnection::handleCommand(std::string &command)
{
    try {
        std::vector<std::string> args = parseCommands(command);
        if (args.empty())
            throw std::runtime_error("No command provided");
        std::string commandName = args[0];
        auto it = commands.find(commandName);
        if (it != commands.end()) {
            (this->*(it->second))(args);
        } else {
            //std::cerr << "Unknown command: " << commandName << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Error handling command: " << e.what() << std::endl;
    }
}

std::vector<std::string> ServerGuiConnection::parseCommands(std::string &command)
{
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

void GUI::ServerGuiConnection::welcomeCommand(std::vector<std::string> &args)
{
    (void) args;
    //sendCommand("GRAPHIC");
}
} // namespace GUI