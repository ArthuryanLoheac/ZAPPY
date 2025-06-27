/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Kick Recovery Module
*/

#ifndef ZAPPY_AI_SRC_MODULES_KICKRECOVERYMODULE_HPP_
#define ZAPPY_AI_SRC_MODULES_KICKRECOVERYMODULE_HPP_

#include <vector>
#include <string>
#include "modules/AIModule.hpp"

/**
 * @brief Module responsible for returning to original position after being kicked
 *
 * This module monitors kick events and executes the necessary movement commands
 * to return to the original tile position.
 */
class KickRecoveryModule : public AIModule {
 public:
    /**
     * @brief Constructor for the KickRecoveryModule
     */
    KickRecoveryModule();

    /**
     * @brief Execute the kick recovery behavior
     */
    void execute() override;

    /**
     * @brief Calculate the priority of this module
     * @return float Priority value between 0.0 and 1.0
     */
    float getPriority() override;

 private:
    /**
     * @brief Convert kick direction to movement commands
     * @param direction Kick direction (1=front, 3=left, 5=back, 7=right)
     * @return std::vector<std::string> List of commands to execute
     */
    std::vector<std::string> getRecoveryCommands(int direction);

    /**
     * @brief Execute the recovery commands for a specific kick
     * @param direction Kick direction
     */
    void executeRecoveryCommands(int direction);

    /// Number of kicks processed to avoid duplicates
    int processedKicks;
};

#endif  // ZAPPY_AI_SRC_MODULES_KICKRECOVERYMODULE_HPP_
