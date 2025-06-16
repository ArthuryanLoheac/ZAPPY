#include <string>
#include <vector>
#include <iostream>

#include "Interface/Interface.hpp"
#include "Exceptions/Commands.hpp"

namespace AI {

void Interface::commandFORWARD(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    (void)command;
    if (args.size() != 1) {
        throw AI::CommandArgumentsException("FORWARD",
            "Expected no arguments, got " +
            std::to_string(args.size() - 1));
    }
}

void Interface::commandLEFT(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    (void)command;
    if (args.size() != 1) {
        throw AI::CommandArgumentsException("LEFT",
            "Expected no arguments, got " +
            std::to_string(args.size() - 1));
    }
}

void Interface::commandRIGHT(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    (void)command;
    if (args.size() != 1) {
        throw AI::CommandArgumentsException("RIGHT",
            "Expected no arguments, got " +
            std::to_string(args.size() - 1));
    }
}

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
