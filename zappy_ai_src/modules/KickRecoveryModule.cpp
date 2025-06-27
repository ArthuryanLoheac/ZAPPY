/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Kick Recovery Module Implementation
*/

#include <string>
#include <vector>
#include "modules/KickRecoveryModule.hpp"
#include "../Logic/AIBase.hpp"
#include "../Interface/Interface.hpp"
#include "../Exceptions/Factory.hpp"
#include "../Exceptions/Commands.hpp"
#include "../Data/Data.hpp"
#include "../../libc/include/logs.h"

/**
 * @brief Initialize the KickRecoveryModule with default values
 */
KickRecoveryModule::KickRecoveryModule() {
    processedKicks = 0;
}

/**
 * @brief Execute the kick recovery behavior
 *
 * Checks for new kicks and executes recovery commands if needed
 */
void KickRecoveryModule::execute() {
    // Check if there are new kicks to process
    if (AI::Data::i().kickVector.size() >
        static_cast<size_t>(processedKicks)) {
        // Process the most recent kick
        auto& latestKick = AI::Data::i().kickVector.back();
        int kickDirection = latestKick.first;

        LOG_INFO("Processing kick with direction: %d", kickDirection);

        // Execute recovery commands
        executeRecoveryCommands(kickDirection);

        // Mark this kick as processed
        processedKicks++;

        LOG_INFO("Kick recovery completed. Total processed kicks: %d",
                processedKicks);
    }
}

/**
 * @brief Calculate the priority of this module
 *
 * High priority when there are unprocessed kicks
 *
 * @return float Priority value between 0.0 and 1.0
 */
float KickRecoveryModule::getPriority() {
    bool hasUnprocessedKicks = AI::Data::i().kickVector.size() >
        static_cast<size_t>(processedKicks);
    float priority = hasUnprocessedKicks ? 1.0f : 0.0f;

    LOG_INFO("Kick Recovery Module Priority: %.2f (unprocessed kicks: %s)",
             priority, hasUnprocessedKicks ? "yes" : "no");

    return priority;
}

/**
 * @brief Convert kick direction to movement commands
 * @param direction Kick direction (1=front, 3=left, 5=back, 7=right)
 * @return std::vector<std::string> List of commands to execute
 */
std::vector<std::string> KickRecoveryModule::getRecoveryCommands(
    int direction) {
    std::vector<std::string> commands;

    switch (direction) {
        case 1:
            commands.push_back(FORWARD);
        break;

        case 3:
        commands.push_back(RIGHT);
        commands.push_back(FORWARD);
        commands.push_back(LEFT);
        break;

        case 5:
            commands.push_back(LEFT);
            commands.push_back(LEFT);
            commands.push_back(FORWARD);
            break;

        case 7:
            commands.push_back(LEFT);
            commands.push_back(FORWARD);
            commands.push_back(RIGHT);
            break;

        default:
            LOG_ERROR("Unknown kick direction: %d", direction);
            break;
    }

    return commands;
}

/**
 * @brief Execute the recovery commands for a specific kick
 * @param direction Kick direction
 */
void KickRecoveryModule::executeRecoveryCommands(int direction) {
    std::vector<std::string> commands = getRecoveryCommands(direction);

    LOG_INFO("Executing %zu recovery commands for kick direction %d",
             commands.size(), direction);

    for (const std::string& command : commands) {
        AI::Interface::i().sendCommand(command);
        LOG_DEBUG("Sent recovery command: %s", command.c_str());
    }
}
