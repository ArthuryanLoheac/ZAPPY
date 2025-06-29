/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Elevation Module
*/

#include "modules/ElevationModule.hpp"

#include <unistd.h>

#include <iostream>
#include <algorithm>
#include <string>

#include "../Interface/Interface.hpp"
#include "../Data/Data.hpp"
#include "../../libc/include/logs.h"

/**
 * @brief Initialize the ElevationModule with default values
 */
ElevationModule::ElevationModule() {
    targetX = -1;
    targetY = -1;
    hasSufficientFood = false;
    hasLinemate = false;
    foundSpot = false;
}

/**
 * @brief Calculate the priority of the elevation module
 *
 * Priority is determined by food availability:
 * - 0.7 if we need to gather food (<3 food)
 * - 0.4 if we have sufficient food (≥3 food)
 * - 0.0 if we have no resources and no conditions for elevation
 *
 * @return float Priority value between 0.0 and 1.0
 */
float ElevationModule::getPriority() {
    foodCount = AI::Data::i().inventory.find(AI::Data::Material_t::Food) !=
        AI::Data::i().inventory.end() ?
        AI::Data::i().inventory.at(AI::Data::Material_t::Food) : 0;
    if (foodCount < 3) {
        LOG_INFO("Elevation Module Priority: 0.7 (not enough food)");
        std::cout << "Not enough food for elevation: " << foodCount
                  << std::endl;
        return 0.7f;
    }
    float ret = elevationPriority;
    elevationPriority -= 0.05f;
    if (elevationPriority < 0.0f)
        elevationPriority = 0.0f;
    std::cout << "Elevation Module Priority: " << ret
              << " with food count: " << foodCount << std::endl;
    return ret;
}

/**
 * @brief Check the resources needed for elevation
 *
 * Updates status flags based on current inventory contents
 */
void ElevationModule::checkResources() {
    foodCount = AI::Data::i().inventory.find(AI::Data::Material_t::Food) !=
        AI::Data::i().inventory.end() ?
        AI::Data::i().inventory.at(AI::Data::Material_t::Food) : 0;

    hasLinemate = AI::Data::i().inventory.find(AI::Data::Material_t::Linemate)
                  != AI::Data::i().inventory.end()
                  && AI::Data::i().inventory.at(AI::Data::Material_t::Linemate)
                  > 0;
    std::cout << "Food count: " << foodCount
              << ", Has linemate: " << hasLinemate << std::endl;

    hasSufficientFood = (foodCount >= 3);
}

/**
 * @brief Execute the elevation module behavior
 *
 * Manages the sequence of looking for linemate, moving to it,
 * and starting incantation
 */
void ElevationModule::execute() {
    elevationPriority = 0.6f;
    std::cout << "Player with PID " << getpid()
              << " executing Elevation Module" << std::endl;
    checkResources();

    static int lookCounter = 0;
    if (++lookCounter >= 10) {
        foundSpot = false;
        lookCounter = 0;
        AI::Interface::i().sendCommand(LOOK);
        return;
    }

    if (!foundSpot && !hasLinemate) {
        std::cout << "Looking for linemate..." << std::endl;
        foundSpot = findElevationSpot();
        if (!foundSpot) {
            AI::Interface::i().sendCommand(LOOK);
            return;
        }
    }

    performElevation();
}

/**
 * @brief Check if linemate is on the current tile
 *
 * @return True if linemate is found on current tile
 */
bool ElevationModule::checkCurrentTileForLinemate() {
    std::cout << "Checking current tile for linemate..." << std::endl;
    if (AI::Data::i().vision.empty() || AI::Data::i().vision[0].empty()) {
        return false;
    }

    const size_t midY = AI::Data::i().vision[0].size() / 2;
    auto& currentTile = AI::Data::i().vision[0][midY];

    if (currentTile.find("linemate") != currentTile.end() &&
        currentTile["linemate"] == 1) {
        LOG_INFO("Found linemate on current tile!");
        targetX = 0;
        targetY = 0;
        std::cout << "Linemate found at (0, 0)!!!!!!!!!!!!!!!!!!!!"
            << std::endl;
        return true;
    }
    std::cout << "No linemate found on current tile." << std::endl;
    return false;
}

/**
 * @brief Scan the vision data for tiles containing linemate
 *
 * @return True if a suitable tile with linemate was found
 */
// This function is now unused and always returns false
bool ElevationModule::scanVisionForLinemate() {
    return false;
}

/**
 * @brief Find a suitable location for elevation
 *
 * @return True if a suitable spot was found
 */
bool ElevationModule::findElevationSpot() {
    if (AI::Data::i().vision.empty()) {
        return false;
    }

    // Only check current tile, do not scan other tiles
    return checkCurrentTileForLinemate();
}

/**
 * @brief Handle the case when linemate is in the player's inventory
 */
void ElevationModule::handleLinemateInInventory() {
    LOG_INFO("Dropping linemate for elevation");
    AI::Interface::i().sendCommand("Set linemate\n");
    foundSpot = false;
    AI::Interface::i().sendCommand(LOOK);
}

/**
 * @brief Drop all items in inventory (except food)
 */
void ElevationModule::dropAllInventory() {
    for (auto &item : AI::Data::i().inventory) {
        if (item.first == AI::Data::Material_t::Food) {
            continue;
        }
        std::string itemName = AI::Data::materialToString(item.first);
        std::transform(itemName.begin(), itemName.end(), itemName.begin(),
            ::tolower);
        int itemCount = item.second;
        for (int i = 0; i < itemCount; ++i) {
            AI::Interface::i().sendCommand("Set " + itemName + "\n");
        }
    }
}

/**
 * @brief Handle the case when linemate was found on a tile
 */
void ElevationModule::handleFoundLinemateTile() {
    // Drop all items before incantation
    dropAllInventory();
    LOG_INFO("Starting incantation");
    AI::Interface::i().sendCommand(INCANTATION);
    foundSpot = false;
    targetX = -1;
    targetY = -1;
}

/**
 * @brief Perform the elevation process
 *
 * This method handles both cases: when linemate is in inventory or when
 * a tile with linemate has been found
 */
void ElevationModule::performElevation() {
    if (foodCount < 3) {
        LOG_WARNING("Not enough food for elevation: %d/3", foodCount);
        return;
    }

    if (hasLinemate) {
        handleLinemateInInventory();
        return;
    }

    // Only trigger incantation if on the correct tile (no movement)
    if (foundSpot && targetX == 0 && targetY == 0) {
        handleFoundLinemateTile();
    }
}

