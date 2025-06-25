/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Disruption Module
*/

#ifndef ZAPPY_AI_SRC_MODULES_DISRUPTIONMODULE_HPP_
#define ZAPPY_AI_SRC_MODULES_DISRUPTIONMODULE_HPP_

#include <random>
#include <unordered_map>
#include <string>
#include "modules/AIModule.hpp"

/**
 * @brief Module for creating random movement patterns
 *
 * This module sends random movement commands (forward, left, right)
 * with an increasing probability of sending LOOK commands.
 */
class DisruptionModule : public AIModule   {
 public:
    /**
     * @brief Constructor for the DisruptionModule
     */
    DisruptionModule();

    /**
     * @brief Execute the disruption behavior
     */
    void execute() override;

    /**
     * @brief Calculate the priority of this module
     * @return float Priority value between 0.0 and 1.0
     */
    float getPriority() override;

 private:
    /**
     * @brief Send a random movement command
     */
    void sendRandomMovement();

    /**
     * @brief Check if a LOOK command should be sent
     * @return true if a LOOK command should be sent
     */
    bool shouldSendLook();

    /**
     * @brief Update the LOOK probability
     * Increases probability by 40% or resets to 0 if at 100%
     */
    void updateLookProbability();

    /**
     * @brief Check if there is an elevation nearby
     * if there is one send the number of the case of the elevation
     */
    int checkVisionElevation();

    /**
     * @brief Get the number of stones in a tile
     * @param tile The tile to check
     * @return int Number of stones in the tile
     */
    int getNumberStone(std::unordered_map<std::string, int> &tile);

    /**
     * @brief Send an attack command to a specific tile
     * @param x The x coordinate of the tile
     * @param relativeY The relative y coordinate of the tile
     */
    void attack(size_t x, int relativeY);

    /// Current probability of sending a LOOK command (0, 40, 80, 100)
    int lookProbability;

    /// Counter for actions taken
    int actionCounter;

    /// Random number generator
    std::mt19937 rng;
};

#endif  // ZAPPY_AI_SRC_MODULES_DISRUPTIONMODULE_HPP_
