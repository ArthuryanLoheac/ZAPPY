#include <string.h>
#include <unistd.h>
#include <sys/poll.h>

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <map>
#include <cstdio>

#include "Connection/ServerAI.hpp"
#include "DataManager/DataManager.hpp"

namespace AI {
ServerAI::ServerAI() {
}

void AI::ServerAI::handleCommand() {
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

void ServerAI::execCommand(std::map<std::string, void(AI::ServerAI::*)
(std::vector<std::string> &)>::iterator it, std::vector<std::string> &args) {
    if (it != commands.end()) {
        try {
            (AI::ServerAI::i().*(it->second))(args);
            if (AI::DataManager::i().getDebug())
                printf("\033[1;32m[OK]\033[0m Received Command: %s\n",
                    args[0].c_str());
        } catch (const std::exception &e) {
            if (AI::DataManager::i().getErrors()) {
                printf("\033[1;31m[ERROR]\033[0m %s : ", e.what());
                for (size_t i = 0; i < args.size(); i++)
                    printf(" %s", args[i].c_str());
                printf("\n");
            }
        }
    } else if (AI::DataManager::i().getErrors()) {
        // Error
        printf("\033[1;31m[ERROR]\033[0m Unknown Command:");
        for (size_t i = 0; i < args.size(); i++)
            printf(" %s", args[i].c_str());
        printf("\n");
    }
}

void ServerAI::readDatasFromServer() {
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

void ServerAI::clockUpdate(std::chrono::_V2::system_clock::time_point &time,
std::chrono::_V2::system_clock::time_point &timeNext) {
    time = std::chrono::system_clock::now();
    if (time >= timeNext) {
        timeNext = timeNext + std::chrono::seconds(updateMapTime);
        sendDatasToServer("mct\n");
    }
}

void ServerAI::startServer() {
    auto time = std::chrono::system_clock::now();
    auto timeNext = time + std::chrono::seconds(updateMapTime);
    int ready = 0;

    while (DataManager::i().running) {
        clockUpdate(time, timeNext);

        ready = poll(
            &AI::ServerAI::i().fd, 1, -1);
        if (ready == -1)
            throw std::runtime_error("Poll error occurred");
        if (fd.revents & POLLIN)
            readDatasFromServer();
    }
}

std::vector<std::string> ServerAI::parseCommands(std::string &command) {
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


void ServerAI::sendDatasToServer(const std::string &message) {
    if (fd.revents & POLLOUT) {
        ssize_t bytes_sent = write(server_fd,
            message.c_str(), message.size());
        if (bytes_sent == -1) {
            throw std::runtime_error("Error sending data to server");
        }
        if (AI::DataManager::i().getDebug())
            printf("Sent data 2: %s\n", message.c_str());
    }
}

}  // namespace AI
