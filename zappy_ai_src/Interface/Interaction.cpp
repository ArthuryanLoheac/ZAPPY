#include <string>
#include <vector>

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
        LOG_ERROR("EJECT: Expected one argument, got %i\n.",
            std::to_string(args.size() - 1).c_str());
        return;
    }
}

}  // namespace AI
