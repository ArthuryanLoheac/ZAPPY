#include <unistd.h>

#include <string>
#include <vector>
#include <iostream>
#include <csignal>

#include "Interface/Interface.hpp"
#include "Exceptions/Commands.hpp"
#include "Data/Data.hpp"
#include "include/logs.h"

/**
 * @file Special.cpp
 * @brief Implementation of special commands like Fork and Incantation
 *
 * This file contains the implementation of commands that have special effects
 * in the game, such as creating new AI instances or performing level-up rituals.
 */

namespace AI {

/**
 * @brief Handles the FORK command response
 *
 * When a fork is successful, signals the parent process to create a new AI instance.
 *
 * @param args Server response arguments
 * @param command Original command sent
 */
void Interface::commandFORK(std::vector<std::string> &args,
std::vector<std::string> &command) {
    if (args.size() != 1) {
        LOG_WARNING("FORK: Expected no arguments, got %i\n.",
            args.size() - 1);
        return;
    }
    (void)command;
}

/**
 * @brief Handles the INCANTATION command response
 *
 * Processes the result of an incantation ritual, updating the AI's level
 * if the incantation was successful.
 *
 * @param args Server response arguments
 * @param command Original command sent
 */
void Interface::commandINCANTATION(std::vector<std::string> &args,
std::vector<std::string> &command) {
    if (args.empty()) {
        LOG_WARNING("INCANTATION: Expected at least one argument, got %i\n.",
            args.size() - 1);
        return;
    }

    (void)command;
    if (args[0] == "KO") {
        Data::i().isInIncantation = false;
        return;
    }
    if (args[0] != "CURRENT") {
        LOG_ERROR("INCANTATION: Expected 'CURRENT' or 'KO', got '%s'\n",
            args[0].c_str());
        return;
    }
    try {
        Data::i().level = std::stoi(args[2]);
    } catch (const std::invalid_argument &e) {
        LOG_ERROR("INCANTATION: Failed to parse level: %s", e.what());
        Data::i().isInIncantation = false;
        return;
    }
}

/**
 * @brief Attempts to start an incantation ritual
 *
 * Sends the incantation command to the server and waits for a response.
 * Updates the AI state if the incantation begins successfully.
 *
 * @return true if the incantation started successfully, false otherwise
 */
bool Interface::startIncantation() {
    if (Data::i().isInIncantation) {
        return false;
    }

    socket.sendDatasToServer(INCANTATION);

    int state = 0;
    while (!state) {
        socket.run();
        auto output = socket.getListOutputs();

        if (output.empty()) {
            continue;
        }

        for (const auto &line : output) {
            if (line[0] == "KO") {
                state = -1;
            } else if (line[0] == "ELEVATION") {
                state = 1;
            } else {
                outputQueue.push(line);
            }
        }
    }
    if (state == -1) return false;
    Data::i().isInIncantation = true;
    inputQueue.push({"INCANTATION"});
    return true;
}

}  // namespace AI
