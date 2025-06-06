#include <unistd.h>
#include <signal.h>

#include <iostream>
#include <string>
#include <vector>

#include "Interface/Interface.hpp"
#include "Exceptions/Factory.hpp"
#include "Exceptions/Commands.hpp"
#include "Data/Data.hpp"

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

    if (outputs.empty()) return;

    for (const auto &output : outputs) {
        if (output[0] == "DEAD") {
            Data::i().isDead = true;
        }
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

static void sendSIGUSR1(pid_t pid) {
    if (pid <= 0) {
        std::cerr << "Invalid PID for SIGUSR1: " << pid << std::endl;
        return;
    }
    if (kill(pid, SIGUSR1) == -1) {
        std::cerr << "Failed to send SIGUSR1 to PID " << pid
            << ": " << strerror(errno) << std::endl;
    }
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
        socket.run();
        auto output = socket.getListOutputs();

        if (output.empty()) {
            continue;
        }

        if (output[0][0] == "KO") {
            std::cerr << "Too much players connected, try again later."
                << std::endl;
            return;
        }

        for (const auto &line : output) {
            followUpCommand.push_back(line);
        }
    }

    try {
        if (followUpCommand[0].size() < 1 || followUpCommand[1].size() < 2) {
            throw AI::CommandArgumentsException("WELCOME",
                "Invalid response format");
        }

        int placeRemaining = std::stoi(followUpCommand[0][0]);

        if (placeRemaining > 0)
            sendSIGUSR1(getppid());

        Data::i().mapX = std::stoi(followUpCommand[1][0]);
        Data::i().mapY = std::stoi(followUpCommand[1][1]);
    } catch (const std::invalid_argument& e) {
        throw AI::CommandArgumentsException("WELCOME",
            "Failed to parse numeric values");
    }
}

}  // namespace AI
