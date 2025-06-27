/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Feeder Module Implementation
*/

#include "modules/FeederModule.hpp"
#include <iostream>
#include "../Interface/Interface.hpp"
#include "../Data/Data.hpp"
#include "../../libc/include/logs.h"

/**
 * @brief Initialize the FeederModule with default values
 */
FeederModule::FeederModule()
    : inventoryCheckCounter(0), foodCount(0),
        currentState(FeederState::DROP_INITIAL_FOOD),
      foodDropCounter(0), forkCommandSent(false), broadcastSent(false) {
    LOG_INFO("Feeder Module initialized in sequence mode");
}

/**
 * @brief Execute the feeding behavior
 *
 * Follows a specific sequence: drop 2 food, fork, drop 1 food, broadcast, then continuous feeding
 */
void FeederModule::execute() {
    if (++inventoryCheckCounter >= 5) {
        checkInventory();
        inventoryCheckCounter = 0;
        return;
    }

    if (currentState == FeederState::CONTINUOUS_FEEDING) {
        executeContinuousFeeding();
        return;
    }

    if (currentState == FeederState::DROP_INITIAL_FOOD && hasFood()) {
        LOG_INFO("Executing initial sequence: Dropping 2 food items");
        AI::Interface::i().sendCommand("Set food\n");
        AI::Interface::i().sendCommand("Set food\n");
        foodDropCounter = 2;

        LOG_INFO("Sending FORK command...");
        AI::Interface::i().sendCommand(FORK);
        forkCommandSent = true;

        LOG_INFO("Dropping extra food item after fork");
        AI::Interface::i().sendCommand("Set food\n");

        LOG_INFO("Broadcasting NEED_FEEDER message...");
        AI::Interface::i().sendMessage("NEED_FEEDER");
        broadcastSent = true;

        LOG_INFO("Moving to continuous feeding phase");
        currentState = FeederState::CONTINUOUS_FEEDING;
        return;
    } else if (currentState == FeederState::DROP_INITIAL_FOOD) {
        LOG_INFO("Waiting for food to execute initial sequence...");
        AI::Interface::i().sendCommand(INVENTORY);
    }
}

/**
 * @brief Execute the initial food dropping phase
 * @return true if completed
 */
bool FeederModule::executeInitialFoodDrop() {
    if (foodDropCounter >= 2) {
        return true;
    }
    if (hasFood()) {
        LOG_INFO("Dropping initial food item %d/2", foodDropCounter + 1);
        AI::Interface::i().sendCommand("Set food\n");
        foodDropCounter++;
        if (foodDropCounter >= 2) {
            return true;
        }
    } else {
        LOG_INFO("Waiting for food to drop initial items...");
    }
    return false;
}

/**
 * @brief Execute the fork command phase
 * @return true if completed
 */
bool FeederModule::executeFork() {
    if (forkCommandSent) {
        return true;
    }
    LOG_INFO("Sending FORK command...");
    AI::Interface::i().sendCommand(FORK);
    forkCommandSent = true;
    return true;
}

/**
 * @brief Execute the extra food drop phase
 * @return true if completed
 */
bool FeederModule::executeExtraFoodDrop() {
    static bool extraFoodDropped = false;
    if (extraFoodDropped) {
        return true;
    }
    if (hasFood()) {
        LOG_INFO("Dropping extra food item after fork");
        AI::Interface::i().sendCommand("Set food\n");
        extraFoodDropped = true;
        return true;
    } else {
        LOG_INFO("Waiting for food to drop extra item...");
    }
    return false;
}

/**
 * @brief Execute the broadcast phase
 * @return true if completed
 */
bool FeederModule::executeBroadcast() {
    if (broadcastSent) {
        return true;
    }
    LOG_INFO("Broadcasting NEED_FEEDER message...");
    AI::Interface::i().sendMessage("NEED_FEEDER");
    broadcastSent = true;
    return true;
}

/**
 * @brief Execute the continuous feeding phase
 */
void FeederModule::executeContinuousFeeding() {
    if (hasFood()) {
        LOG_INFO("Continuously feeding: placing food on ground...");
        AI::Interface::i().sendCommand("Set food\n");
    } else {
        LOG_INFO("No food available for continuous feeding...");
    }
}

/**
 * @brief Calculate the priority of this module
 * @return float Priority value between 0.0 and 1.0
 */
float FeederModule::getPriority() {
    // No other module, so highest priority
    return 0.1f;
}

/**
 * @brief Check if there's food in inventory
 * @return True if there's at least one food item
 */
bool FeederModule::hasFood() const {
    return foodCount > 0;
}

/**
 * @brief Check inventory periodically to know current food level
 */
void FeederModule::checkInventory() {
    AI::Interface::i().sendCommand(INVENTORY);
    foodCount = AI::Data::i().inventory.find(AI::Data::Material_t::Food) !=
                AI::Data::i().inventory.end() ?
                AI::Data::i().inventory.at(AI::Data::Material_t::Food) : 0;
}

/**
 * @brief Get the current state of the feeder sequence
 * @return Current state enum value
 */
FeederState FeederModule::getCurrentState() const {
    return currentState;
}
