#include <string>
#include <vector>
#include <stdexcept>

#include "Interface/Interface.hpp"
#include "Exceptions/Commands.hpp"
#include "Data/Data.hpp"
#include "include/logs.h"

/**
 * @file Interaction.cpp
 * @brief Implementation of interaction-related command handlers
 *
 * This file contains the implementation of commands that involve
 * interaction with other players or the environment, like ejecting players.
 */

namespace AI {

/**
 * @brief Handles the EJECT command response
 *
 * Processes the response from the server after attempting to eject other
 * players from the current tile.
 *
 * @param args Server response arguments
 * @param command Original command sent
 */
void Interface::commandEJECT(std::vector<std::string> &args,
std::vector<std::string> &command) {
    (void)command;
    if (args.size() != 1) {
        LOG_WARNING("EJECT: Expected one argument, got %i\n.",
            args.size() - 1);
        return;
    }
}

void Interface::receiveEject(const std::vector<std::string> &args) {
    if (args.size() < 2) {
        LOG_WARNING("EJECT: Invalid arguments received");
        return;
    }

    int direction;
    try {
        direction = std::stoi(args[1]);
    } catch (const std::invalid_argument &e) {
        LOG_WARNING("EJECT: Invalid argument for direction: %s",
            args[1].c_str());
        return;
    } catch (const std::out_of_range &e) {
        LOG_WARNING("EJECT: Direction value out of range: %s", args[1].c_str());
        return;
    }
    if (direction < 1 || direction > 8) {
        LOG_WARNING("EJECT: Invalid direction value %d", direction);
        return;
    }

    Data::i().kickVector.push_back({direction, 0});
    LOG_INFO("EJECT: Player ejected in direction %d", direction);
}

}  // namespace AI
