/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Feeder Module
*/

#ifndef ZAPPY_AI_SRC_MODULES_FEEDERMODULE_HPP_
#define ZAPPY_AI_SRC_MODULES_FEEDERMODULE_HPP_

#include "modules/AIModule.hpp"

// Enum to track the feeder's current state in the sequence
enum class FeederState {
    DROP_INITIAL_FOOD,  // State 1: Drop the first 2 food items
    FORK_NEW_AI,        // State 2: Fork a new AI
    DROP_EXTRA_FOOD,    // State 3: Drop 1 more food item
    BROADCAST_NEED,     // State 4: Broadcast need for feeder
    CONTINUOUS_FEEDING  // State 5: Continuously drop food
};

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

    /**
     * @brief Get the current state of the feeder sequence
     * @return Current state enum value
     */
    FeederState getCurrentState() const;

 private:
    /**
     * @brief Check if there's food in inventory
     * @return True if there's at least one food item
     */
    bool hasFood() const;

    /**
     * @brief Get current food count
     * @return Current food count
     */
    int getFoodCount() const {
        return foodCount;
    }

    /**
     * @brief Check inventory periodically to know current food level
     */
    void checkInventory();

    /**
     * @brief Execute the initial food dropping phase
     * @return true if completed
     */
    bool executeInitialFoodDrop();

    /**
     * @brief Execute the fork command phase
     * @return true if completed
     */
    bool executeFork();

    /**
     * @brief Execute the extra food drop phase
     * @return true if completed
     */
    bool executeExtraFoodDrop();

    /**
     * @brief Execute the broadcast phase
     * @return true if completed
     */
    bool executeBroadcast();

    /**
     * @brief Execute the continuous feeding phase
     */
    void executeContinuousFeeding();

    /// Counter for inventory checks
    int inventoryCheckCounter;

    /// Food count in inventory
    int foodCount;

    /// Current state in the feeding sequence
    FeederState currentState;

    /// Counter for food dropped in the current state
    int foodDropCounter;

    /// Flag to track if fork command was sent
    bool forkCommandSent;

    /// Flag to track if broadcast was sent
    bool broadcastSent;
};

#endif  // ZAPPY_AI_SRC_MODULES_FEEDERMODULE_HPP_
