#include <string.h>
#include <unistd.h>
#include <sys/poll.h>

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

#include "Connection/ServerGUI.hpp"
#include "DataManager/DataManager.hpp"

namespace GUI {
ServerGUI::ServerGUI() {
}

void GUI::ServerGUI::handleCommand() {
    while (buffer.find("\n") != std::string::npos) {
        size_t pos = buffer.find("\n");
        std::string command = buffer.substr(0, pos);
        buffer.erase(0, pos + 1);

        if (command.empty()) continue;

        std::vector<std::string> args =
            parseCommands(command);
        if (args.empty()) continue;

        auto it = commands.find(args[0]);
        if (it != commands.end())
            (GUI::ServerGUI::i().*(it->second))(args);
        else if (GUI::DataManager::i().getDebug())
            std::cerr << "Unknown command: " << args[0] << std::endl;
    }
}

void ServerGUI::readDatasFromServer() {
    char bufferTemp[1024];
    ssize_t bytes_read = 0;

    bytes_read = read(server_fd,
        bufferTemp, sizeof(bufferTemp) - 1);
    if (bytes_read <= 0)
        throw std::runtime_error("Error reading from server");
    bufferTemp[bytes_read] = '\0';
    buffer.append(bufferTemp);
    handleCommand();
}

void ServerGUI::startServer() {
    int ready = 0;

    while (1) {
        ready = poll(
            &GUI::ServerGUI::i().fd, 1, -1);
        if (ready == -1)
            throw std::runtime_error("Poll error occurred");
        if (fd.revents & POLLIN)
            readDatasFromServer();
    }
}

std::vector<std::string> ServerGUI::parseCommands(std::string &command) {
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
    if (fd.revents & POLLOUT) {
        ssize_t bytes_sent = write(server_fd,
            message.c_str(), message.size());
        if (bytes_sent == -1) {
            throw std::runtime_error("Error sending data to server");
        }
        printf("Sent data: %s\n", message.c_str());
    }
}

}  // namespace GUI
