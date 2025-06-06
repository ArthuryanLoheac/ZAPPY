#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "Interface/Interface.hpp"
#include "Exceptions/Factory.hpp"
#include "Exceptions/Commands.hpp"

namespace AI {

Interface::Interface() {
    factoryCommands();
}

Interface::~Interface() {
}

void Interface::start(int port, const std::string &ip,
    const std::string &name) {
    this->port = port;
    this->ip = ip;
    this->name = name;
    try {
        socket.startSocket(port, ip);
    } catch (const std::exception &e) {
        throw AI::ChildProcessException(
            "Error starting socket in child process: " + std::string(e.what()));
    }
    inputQueue.push({"WELCOME"});
}

void Interface::stop() {
    try {
        socket.stopSocket();
    } catch (const std::exception &e) {
        throw AI::ChildProcessException(
            "Error stopping socket in child process: " + std::string(e.what()));
    }
}

void Interface::run() {
    try {
        socket.run();
    } catch (const std::exception &e) {
        throw AI::ChildProcessException(
            "Error running socket in child process: " + std::string(e.what()));
    }
    auto outputs = socket.getListOutputs();
    if (outputs.empty()) {
        return;
    }
    for (const auto &output : outputs) {
        outputQueue.push(output);
    }
    try {
        handleQueues();
    } catch (const AI::CommandNotFoundException &e) {
        std::cerr << "Command not found: " << e.what() << std::endl;
    } catch (const AI::CommandArgumentsException &e) {
        std::cerr << "Invalid command arguments: " << e.what() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
    }
}

void Interface::handleQueues() {
    for (size_t i = 0; i < outputQueue.size(); i++) {
        auto input = inputQueue.front();
        inputQueue.pop();
        auto output = outputQueue.front();
        outputQueue.pop();
        if (commands.find(input[0]) == commands.end()) {
            throw AI::CommandNotFoundException(input[0]);
        }
        try {
            (this->*(commands[input[0]]))(output);
        } catch (const std::exception &e) {
            std::cerr << "Error handling command '" << input[0]
                << "': " << e.what() << std::endl;
        }
    }
}

void Interface::factoryCommands() {
    commands["WELCOME"] = &Interface::commandWELCOME;
}

void Interface::commandWELCOME(std::vector<std::string> &args) {
    if (args.size() != 1) {
        throw AI::CommandArgumentsException("WELCOME",
            "Expected no arguments, got " +
            std::to_string(args.size() - 1));
    }
    socket.sendDatasToServer(name + "\n");
    std::vector<std::vector<std::string>> followUpCommand;
    while (followUpCommand.size() < 2) {
        auto output = socket.getListOutputs();
        if (output.empty()) continue;
        if (output[0][0] == "ko") {
            std::cerr << "Too much players connected, try again later."
                << std::endl;
        }
        for (const auto &line : output) {
            followUpCommand.push_back(line);
        }
    }
}

}  // namespace AI
