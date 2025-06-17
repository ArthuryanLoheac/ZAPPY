#include <unistd.h>

#include <csignal>
#include <iostream>
#include <string>
#include <vector>

#include "Interface/Interface.hpp"
#include "Exceptions/Factory.hpp"
#include "Exceptions/Commands.hpp"
#include "Data/Data.hpp"

/**
 * @file Interface.cpp
 * @brief Implementation of the main Interface class methods
 *
 * This file contains the core implementation of the Interface class which
 * handles all communication with the Zappy server and coordinates
 * the processing of commands and responses.
 */

namespace AI {

Interface::Interface() : port(0) {
    factoryCommands();
    initializeFilteredStrings();
}

Interface::~Interface() {
}

static std::string computeMagicKey(const std::string &name) {
    int asciiSum = 0;

    for (char c : name) {
        asciiSum += static_cast<int>(c);
    }
    asciiSum = asciiSum % 256;

    return std::to_string(asciiSum);
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
    Data::i().magicKey = computeMagicKey(name);
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
        if (output[0] == "MESSAGE") {
            auto outputCopy = output;
            receiveMessage(outputCopy);
            continue;
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

    if (commandBuffer.empty()) {
        return;
    }

    for (size_t i = 0; i < (10 - inputQueue.size()); i++) {
        auto command = commandBuffer.front();
        commandBuffer.pop();
        sendCommand(command);
    }
}

// ————————————————————————— Receiving commands ————————————————————————

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
            (this->*(commands[input[0]]))(output, input);
        } catch (const std::exception &e) {
            std::cerr << "Error handling command '" << input[0]
                << "': " << e.what() << std::endl;
        }
    }
}

// ————————————————————————— Sending commands ————————————————————————

std::vector<std::string> parseCommands(const std::string &command) {
    std::vector<std::string> args;
    std::string cmd = command;
    size_t pos = 0;

    while ((pos = cmd.find(' ')) != std::string::npos) {
        args.push_back(cmd.substr(0, pos));
        cmd.erase(0, pos + 1);
    }
    args.push_back(cmd);
    return args;
}

std::string toUpperCase(const std::string &str) {
    std::string upperStr = str;
    for (auto &c : upperStr) {
        c = toupper(c);
    }
    return upperStr;
}

void Interface::sendCommand(const std::string &command) {
    if (inputQueue.size() >= 10) {
        commandBuffer.push(command);
        return;
    }
    socket.sendDatasToServer(command);
    auto parsedCommand = parseCommands(command.substr(0, command.size()-1));
    parsedCommand[0] = toUpperCase(parsedCommand[0]);
    inputQueue.push(parsedCommand);
}

// ————————————————————————— END OF INIT CLASS ————————————————————————

void Interface::factoryCommands() {
    commands["WELCOME"] = &Interface::commandWELCOME;
    commands["FORWARD"] = &Interface::commandFORWARD;
    commands["LEFT"] = &Interface::commandLEFT;
    commands["RIGHT"] = &Interface::commandRIGHT;
    commands["LOOK"] = &Interface::commandLOOK;
    commands["INVENTORY"] = &Interface::commandINVENTORY;
    commands["TAKE"] = &Interface::commandTAKE;
    commands["SET"] = &Interface::commandSET;
    commands["EJECT"] = &Interface::commandEJECT;
    commands["FORK"] = &Interface::commandFORK;
    commands["BROADCAST"] = &Interface::commandBROADCAST;
    commands["INCANTATION"] = &Interface::commandINCANTATION;
}

// ————————————————————————— END OF FACTORY ————————————————————————

void Interface::commandWELCOME(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    (void)command;
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
            Data::i().isDead = true;
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

        if (std::stoi(followUpCommand[0][0]) != 1) {
            kill(getppid(), SIGUSR1);
        }

        Data::i().mapX = std::stoi(followUpCommand[1][0]);
        Data::i().mapY = std::stoi(followUpCommand[1][1]);
        Data::i().isRunning = true;
    } catch (const std::invalid_argument& e) {
        throw AI::CommandArgumentsException("WELCOME",
            "Failed to parse numeric values");
    }
}

}  // namespace AI
