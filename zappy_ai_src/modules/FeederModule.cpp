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
    : inventoryCheckCounter(0), foodCount(0), statusCounter(0) {
    std::cout << "Feeder Module initialized" << std::endl;
}

/**
 * @brief Execute the feeding behavior
 *
 * Continuously sets food on the ground
 */
void FeederModule::execute() {
    // Check inventory periodically
    if (++inventoryCheckCounter >= 5) {
        checkInventory();
        inventoryCheckCounter = 0;
        return;
    }
    
    // Broadcast status occasionally
    if (++statusCounter >= 30) {
        statusCounter = 0;
        std::cout << "Feeder status: Food count = " << foodCount << std::endl;
        AI::Interface::i().sendMessage("FEEDER_STATUS_" + std::to_string(foodCount));
    }
    
    // If we have food, place it on the ground
    if (hasFood()) {
        std::cout << "Feeder placing food on ground..." << std::endl;
        AI::Interface::i().sendCommand("Set food\n");
    }
}

/**
 * @brief Calculate the priority of this module
 * @return float Priority value between 0.0 and 1.0
 */
float FeederModule::getPriority() {
    // Always high priority - this is the feeder's primary job
    return 0.9f;
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
