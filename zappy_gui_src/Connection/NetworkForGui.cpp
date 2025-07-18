#include <string.h>
#include <unistd.h>
#include <sys/poll.h>

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <map>
#include <cstdio>

#include "Connection/NetworkForGui.hpp"
#include "Connection/PollWrapper.hpp"
#include "DataManager/DataManager.hpp"
#include "DataManager/GameDataManager.hpp"
#include "Window/window.hpp"

#include "include/logs.h"

namespace GUI {
NetworkForGui::NetworkForGui() : pollWrapper() {
}

void NetworkForGui::InitServer() {
    toClear = true;
    GUI::GameDataManager::i().clear();
    GUI::DataManager::i().clear();
    GUI::NetworkForGui::i().setConnectedToServer(true);
}

void GUI::NetworkForGui::handleCommand() {
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

void NetworkForGui::execCommand(
std::map<std::string, void(GUI::NetworkForGui::*)(std::vector<std::string> &)>
::iterator it, std::vector<std::string> &args) {
    std::string errStr;

    if (it != commands.end()) {
        try {
            (GUI::NetworkForGui::i().*(it->second))(args);
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
}

void NetworkForGui::readDatasFromServer() {
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

void NetworkForGui::clockUpdate(std::chrono::system_clock::time_point &time,
std::chrono::system_clock::time_point &timeNext,
std::chrono::system_clock::time_point &timeNextPing) {
    time = std::chrono::system_clock::now();
    if (time >= timeNext && !GUI::GameDataManager::i().getGameOver()) {
        timeNext = timeNext + std::chrono::seconds(updateMapTime);
        sendDatasToServer("mct\n");
    } else if (time >= timeNextPing) {
        timeNextPing = timeNextPing + std::chrono::seconds(updatePingTime);
        sendDatasToServer("PING\n");
        sendPing = true;
        timeForPing = std::chrono::system_clock::time_point(time);
    }
}

void NetworkForGui::startServer() {
    auto time = std::chrono::system_clock::now();
    auto timeNext = time + std::chrono::seconds(updateMapTime);
    auto timeNextPing = time + std::chrono::milliseconds(1);

    InitServer();
    while (DataManager::i().running) {
        clockUpdate(time, timeNext, timeNextPing);

        pollWrapper.waitForEvent();
        if (pollWrapper.isReadable())
            readDatasFromServer();
        if (pollWrapper.isWritable() && !outbuffer.empty()) {
            sendBufferToServer();
        }
    }
}

bool NetworkForGui::isConnectedToServer() const {
    return isConnected;
}

void NetworkForGui::setConnectedToServer(bool connected) {
    isConnected = connected;
}

std::vector<std::string> NetworkForGui::parseCommands(std::string &command) {
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

void NetworkForGui::sendBufferToServer() {
    ssize_t bytes_sent = write(server_fd,
        outbuffer.c_str(), outbuffer.size());
    if (bytes_sent <= 0)
        throw std::runtime_error("Error sending data to server");
    outbuffer.erase(0, bytes_sent);
    LOG_DEBUG("[OK] Sent data: %s\n", outbuffer.c_str());
}

void NetworkForGui::sendDatasToServer(const std::string &message) {
    outbuffer += message;
}

void NetworkForGui::initNetwork(int sockfd) {
    server_fd = sockfd;
    pollWrapper.fd = {server_fd, .events = POLLIN | POLLOUT, 0};
}

}  // namespace GUI
