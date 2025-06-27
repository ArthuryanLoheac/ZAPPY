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

/**
 * @brief Initialize the FeederModule with default values
 */
FeederModule::FeederModule()
    : inventoryCheckCounter(0), foodCount(0),
        currentState(FeederState::DROP_INITIAL_FOOD),
      foodDropCounter(0), forkCommandSent(false), broadcastSent(false) {
    std::cout << "Feeder Module initialized in sequence mode" << std::endl;
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
        std::cout << "Executing initial sequence: Dropping 2 food items"
            << std::endl;
        AI::Interface::i().sendCommand("Set food\n");
        AI::Interface::i().sendCommand("Set food\n");
        foodDropCounter = 2;

        std::cout << "Sending FORK command..." << std::endl;
        AI::Interface::i().sendCommand(FORK);
        forkCommandSent = true;

        std::cout << "Dropping extra food item after fork" << std::endl;
        AI::Interface::i().sendCommand("Set food\n");

        std::cout << "Broadcasting NEED_FEEDER message..." << std::endl;
        AI::Interface::i().sendMessage("NEED_FEEDER");
        broadcastSent = true;

        std::cout << "Moving to continuous feeding phase" << std::endl;
        currentState = FeederState::CONTINUOUS_FEEDING;
        return;
    } else if (currentState == FeederState::DROP_INITIAL_FOOD) {
        std::cout << "Waiting for food to execute initial sequence..."
            << std::endl;
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
        std::cout << "Dropping initial food item " << (foodDropCounter + 1)
            << "/2" << std::endl;
        AI::Interface::i().sendCommand("Set food\n");
        foodDropCounter++;
        if (foodDropCounter >= 2) {
            return true;
        }
    } else {
        std::cout << "Waiting for food to drop initial items..." << std::endl;
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
    std::cout << "Sending FORK command..." << std::endl;
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
        std::cout << "Dropping extra food item after fork" << std::endl;
        AI::Interface::i().sendCommand("Set food\n");
        extraFoodDropped = true;
        return true;
    } else {
        std::cout << "Waiting for food to drop extra item..." << std::endl;
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
    std::cout << "Broadcasting NEED_FEEDER message..." << std::endl;
    AI::Interface::i().sendMessage("NEED_FEEDER");
    broadcastSent = true;
    return true;
}

/**
 * @brief Execute the continuous feeding phase
 */
void FeederModule::executeContinuousFeeding() {
    if (hasFood()) {
        std::cout << "Continuously feeding: placing food on ground..."
            << std::endl;
        AI::Interface::i().sendCommand("Set food\n");
    } else {
        std::cout << "No food available for continuous feeding..."
            << std::endl;
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
    foodCount = AI::Data::i().inventory.find("food") !=
                AI::Data::i().inventory.end() ?
                AI::Data::i().inventory.at("food") : 0;
}

/**
 * @brief Get the current state of the feeder sequence
 * @return Current state enum value
 */
FeederState FeederModule::getCurrentState() const {
    return currentState;
}
