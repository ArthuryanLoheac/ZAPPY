#include <string>
#include <vector>
#include <iostream>

#include "Interface/Interface.hpp"
#include "Exceptions/Commands.hpp"
#include "include/logs.h"

/**
 * @file Mouvement.cpp
 * @brief Implementation of movement-related command handlers
 *
 * This file contains the implementation of commands that control
 * the AI's movement on the game map, such as moving forward and turning.
 */

namespace AI {

/**
 * @brief Handles the FORWARD command response
 *
 * Processes the response from the server after attempting to move forward.
 *
 * @param args Server response arguments
 * @param command Original command sent
 */
void Interface::commandFORWARD(std::vector<std::string> &args,
std::vector<std::string> &command) {
    (void)command;
    if (args.size() != 1) {
        LOG_ERROR("FORWARD: Expected no arguments, got %i\n.",
            args.size() - 1);
        return;
    }
}

/**
 * @brief Handles the LEFT command response
 *
 * Processes the response from the server after attempting to turn left.
 *
 * @param args Server response arguments
 * @param command Original command sent
 */
void Interface::commandLEFT(std::vector<std::string> &args,
std::vector<std::string> &command) {
    (void)command;
    if (args.size() != 1) {
        LOG_ERROR("LEFT: Expected no arguments, got %i\n.",
            args.size() - 1);
        return;
    }
}

/**
 * @brief Handles the RIGHT command response
 *
 * Processes the response from the server after attempting to turn right.
 *
 * @param args Server response arguments
 * @param command Original command sent
 */
void Interface::commandRIGHT(std::vector<std::string> &args,
std::vector<std::string> &command) {
    (void)command;
    if (args.size() != 1) {
        LOG_ERROR("RIGHT: Expected no arguments, got %i\n.",
            args.size() - 1);
        return;
    }
}

/**
 * @brief Checks if the AI is waiting for a response from the server
 * @return true if waiting for a response, false otherwise
 */
bool Interface::isWaitingForResponse() const {
    return !inputQueue.empty() || !commandBuffer.empty();
}

/**
 * @brief Moves the AI to a specific position on the map, based on the system of the inventory direction
 * @param x The x-coordinate to move to
 * @param y The y-coordinate to move to
 */
void Interface::goTo(int x, int y) {
    if (x < 0) {
        sendCommand(LEFT);
        sendCommand(LEFT);
        x = -x;
    }

    for (int i = 0; i < x; ++i) {
        sendCommand(FORWARD);
    }
    if (y < 0) {
        sendCommand(LEFT);
        for (int i = 0; i < -y; ++i) {
            sendCommand(FORWARD);
        }
    } else if (y > 0) {
        sendCommand(RIGHT);
        for (int i = 0; i < y; ++i) {
            sendCommand(FORWARD);
        }
    }
}

}  // namespace AI
