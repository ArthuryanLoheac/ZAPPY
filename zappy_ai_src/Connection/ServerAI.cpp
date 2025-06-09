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
#include "ServerAI.hpp"

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
            if (AI::DataManager::i().getDebug())
                printf("\033[1;32m[OK]\033[0m Received Command: %s\n",
                    args[0].c_str());
            (AI::ServerAI::i().*(it->second))(args);
        } catch (const std::exception &e) {
            handleReturnValue(args);
            if (AI::DataManager::i().getErrors()) {
                printf("\033[1;31m[ERROR]\033[0m %s : ", e.what());
                for (size_t i = 0; i < args.size(); i++)
                    printf(" %s", args[i].c_str());
                printf("\n");
            }
        }
    } else  {
        handleReturnValue(args);
    }
}

void ServerAI::execReturnCommand(std::map<std::string, void(AI::ServerAI::*)
(std::vector<std::string> &)>::iterator it, std::vector<std::string> &args) {
    if (it != LastCommands.end()) {
        try {
            (AI::ServerAI::i().*(it->second))(args);
        } catch (const std::exception &e) {}
    } else {
        returnWelcomeCommand(args);
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

void ServerAI::startServer() {
    int ready = 0;

    while (DataManager::i().running) {
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
            printf("[OK] Sent data: %s\n", message.c_str());
        lastCommand = message;
    }
}

void ServerAI::handleReturnValue(std::vector<std::string> &args)
{
    if (args.empty()) return;
    printf("RECEIVED RETURN : %s\n", args[0].c_str());
    auto it = LastCommands.find(lastCommand);
    execReturnCommand(it, args);
}

} // namespace AI
