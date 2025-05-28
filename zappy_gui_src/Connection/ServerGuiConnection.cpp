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

void GUI::ServerGuiConnection::handleCommand()
{
    while (GUI::ServerGuiConnection::i().buffer.find("\n") != std::string::npos) {
        size_t pos = GUI::ServerGuiConnection::i().buffer.find("\n");
        std::string command = GUI::ServerGuiConnection::i().buffer.substr(0, pos);
        GUI::ServerGuiConnection::i().buffer.erase(0, pos + 1);

        if (command.empty())
            continue;

        std::vector<std::string> args = GUI::ServerGuiConnection::i().parseCommands(command);
        if (args.empty())
            continue;

        auto it = GUI::ServerGuiConnection::i().commands.find(args[0]);
        if (it != GUI::ServerGuiConnection::i().commands.end()) {
            (GUI::ServerGuiConnection::i().*(it->second))(args);
        } else {
            std::cerr << "Unknown command: " << args[0] << std::endl;
        }
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

void ServerGuiConnection::sendDatasToServer(const std::string &message) {

    if (GUI::ServerGuiConnection::i().fd.revents & POLLOUT) {
        ssize_t bytes_sent = write(GUI::ServerGuiConnection::i().server_fd, message.c_str(), message.size());
        if (bytes_sent == -1) {
            throw std::runtime_error("Error sending data to server");
        }
        printf("Sent data: %s\n", message.c_str());
    }
}

} // namespace GUI