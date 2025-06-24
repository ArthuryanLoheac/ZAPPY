/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Feeder Module
*/

#ifndef ZAPPY_AI_SRC_MODULES_FEEDERMODULE_HPP_
#define ZAPPY_AI_SRC_MODULES_FEEDERMODULE_HPP_

#include "modules/AIModule.hpp"

/**
 * @brief Module for providing food to other players
 *
 * This module continuously places food on the ground to be
 * collected by other players. It serves as a "feeder" for the team.
 */
class FeederModule : public AIModule {
 public:
    /**
     * @brief Constructor for the FeederModule
     */
    FeederModule();

    /**
     * @brief Execute the feeding behavior
     */
    void execute() override;

    /**
     * @brief Calculate the priority of this module
     * @return float Priority value between 0.0 and 1.0
     */
    float getPriority() override;

 private:
    /**
     * @brief Check if there's food in inventory
     * @return True if there's at least one food item
     */
    bool hasFood() const;

    /**
     * @brief Check inventory periodically to know current food level
     */
    void checkInventory();

    /// Counter for inventory checks
    int inventoryCheckCounter;
    
    /// Food count in inventory
    int foodCount;
    
    /// Counter for status broadcasts
    int statusCounter;
};

#endif  // ZAPPY_AI_SRC_MODULES_FEEDERMODULE_HPP_
