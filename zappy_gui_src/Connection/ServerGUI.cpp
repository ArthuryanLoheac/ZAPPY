#include <string.h>
#include <unistd.h>
#include <sys/poll.h>

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <map>
#include <cstdio>

#include "Connection/ServerGUI.hpp"
#include "DataManager/DataManager.hpp"
#include "include/logs.h"

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

        for (size_t i = 0; i < args[0].length(); ++i)
            args[0][i] = toupper(args[0][i]);

        auto it = commands.find(args[0]);
        execCommand(it, args);
    }
}

void ServerGUI::execCommand(std::map<std::string, void(GUI::ServerGUI::*)
(std::vector<std::string> &)>::iterator it, std::vector<std::string> &args) {
    std::string errStr;

    if (it != commands.end()) {
        try {
            (GUI::ServerGUI::i().*(it->second))(args);
            std::string logMessage = "\033[1;32m[OK]\033[0m Received Command: "
                + args[0];
            for (size_t i = 1; i < args.size(); i++)
                logMessage += " " + args[i];
            LOG_DEBUG(logMessage.c_str());
        } catch (const std::exception &e) {
            errStr += std::string(e.what()) + " : ";
            for (size_t i = 0; i < args.size(); i++)
                errStr += " " + args[i];
            LOG_WARNING(errStr.c_str());
        }
    } else {
        // Error
        errStr += "Unknown Command: ";
        for (size_t i = 0; i < args.size(); i++)
            errStr += " " + args[i];
        LOG_WARNING(errStr.c_str());
    }
    printf("end of command: %s\n", args[0].c_str());
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

void ServerGUI::clockUpdate(std::chrono::system_clock::time_point &time,
std::chrono::system_clock::time_point &timeNext) {
    time = std::chrono::system_clock::now();
    if (time >= timeNext) {
        timeNext = timeNext + std::chrono::seconds(updateMapTime);
        sendDatasToServer("mct\n");
    }
}

void ServerGUI::startServer() {
    auto time = std::chrono::system_clock::now();
    auto timeNext = time + std::chrono::seconds(updateMapTime);
    int ready = 0;

    GUI::ServerGUI::i().setConnectedToServer(true);
    while (DataManager::i().running) {
        clockUpdate(time, timeNext);

        ready = poll(
            &GUI::ServerGUI::i().fd, 1, -1);
        if (ready == -1)
            throw std::runtime_error("Poll error occurred");
        if (fd.revents & POLLIN)
            readDatasFromServer();
        if (fd.revents & POLLOUT && !outbuffer.empty()) {
            sendDatasToServer(outbuffer);
            outbuffer.clear();
        }
    }
}

bool ServerGUI::isConnectedToServer() const {
    return isConnected;
}

void ServerGUI::setConnectedToServer(bool connected) {
    isConnected = connected;
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
        LOG_WARNING("Empty command received.");
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
        LOG_DEBUG("[OK] Sent data: %s\n", message.c_str());
    }
}

}  // namespace GUI
