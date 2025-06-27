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
        LOG_WARNING("FORWARD: Expected no arguments, got %i\n.",
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
        LOG_WARNING("LEFT: Expected no arguments, got %i\n.",
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
        LOG_WARNING("RIGHT: Expected no arguments, got %i\n.",
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

/**
 * @brief Some memes don't hurt
 */
void Interface::doABarrelRoll() {
    sendCommand(LEFT);
    sendCommand(LEFT);
}

void Interface::goToDirection(int direction) {
    if (direction < 0 || direction > 8) {
        LOG_WARNING("goToDirection: Expected a direction between 0 & 8"
            "but got %i", direction);
        return;
    }
    switch (direction) {
        case 0:
            return;
        case 1:
            sendCommand(FORWARD);
            return;
        case 2:
            sendCommand(FORWARD);
            break;
        case 3:
            sendCommand(LEFT);
            sendCommand(FORWARD);
            break;
        case 4:
            sendCommand(LEFT);
            sendCommand(FORWARD);
            break;
        case 5:
            doABarrelRoll();
            sendCommand(FORWARD);
            break;
        case 6:
            sendCommand(RIGHT);
            sendCommand(FORWARD);
            break;
        case 7:
            sendCommand(RIGHT);
            sendCommand(FORWARD);
            break;
        case 8:
            sendCommand(FORWARD);
            break;
    }
}

}  // namespace AI
