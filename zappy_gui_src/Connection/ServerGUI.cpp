#include <string.h>
#include <unistd.h>
#include <sys/poll.h>

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

#include "Connection/ServerGUI.hpp"

namespace GUI {
ServerGUI::ServerGUI() {
}

void GUI::ServerGUI::handleCommand() {
    while (GUI::ServerGUI::i().buffer.find("\n") != std::string::npos) {
        size_t pos = GUI::ServerGUI::i().buffer.find("\n");
        std::string command =
            GUI::ServerGUI::i().buffer.substr(0, pos);
        GUI::ServerGUI::i().buffer.erase(0, pos + 1);

        if (command.empty())
            continue;

        std::vector<std::string> args =
            GUI::ServerGUI::i().parseCommands(command);
        if (args.empty())
            continue;

        auto it = GUI::ServerGUI::i().commands.find(args[0]);
        if (it != GUI::ServerGUI::i().commands.end()) {
            (GUI::ServerGUI::i().*(it->second))(args);
        } else {
            std::cerr << "Unknown command: " << args[0] << std::endl;
        }
    }
}

std::vector<std::string> ServerGUI::parseCommands
(std::string &command) {
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

void ServerGUI::sendDatasToServer(const std::string &message) {
    if (GUI::ServerGUI::i().fd.revents & POLLOUT) {
        ssize_t bytes_sent = write(GUI::ServerGUI::i().server_fd,
            message.c_str(), message.size());
        if (bytes_sent == -1) {
            throw std::runtime_error("Error sending data to server");
        }
        printf("Sent data: %s\n", message.c_str());
    }
}

}  // namespace GUI
