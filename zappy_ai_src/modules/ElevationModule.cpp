/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Elevation Module
*/

#include "modules/ElevationModule.hpp"
#include <iostream>
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
    checkResources();

    if (foodCount < 3)
        return 1.0f;

    return (hasSufficientFood) ? 0.4f : 0.7f;
}

/**
 * @brief Check the resources needed for elevation
 *
 * Updates status flags based on current inventory contents
 */
void ElevationModule::checkResources() {
    foodCount = AI::Data::i().inventory.find("food") !=
        AI::Data::i().inventory.end() ?
        AI::Data::i().inventory.at("food") : 0;

    hasLinemate = AI::Data::i().inventory.find("linemate") !=
                  AI::Data::i().inventory.end() &&
                  AI::Data::i().inventory.at("linemate") > 0;

    hasSufficientFood = (foodCount >= 3);
}

/**
 * @brief Execute the elevation module behavior
 *
 * Manages the sequence of looking for linemate, moving to it, and starting incantation
 */
void ElevationModule::execute() {
    checkResources();

    static int lookCounter = 0;
    if (++lookCounter >= 10) {
        foundSpot = false;
        lookCounter = 0;
        AI::Interface::i().sendCommand(LOOK);
        return;
    }

    if (!foundSpot && !hasLinemate) {
        foundSpot = findElevationSpot();
        if (!foundSpot) {
            AI::Interface::i().sendCommand(LOOK);
            return;
        }
    }

    if (foundSpot || hasLinemate) {
        performElevation();
    }
}

/**
 * @brief Check if linemate is on the current tile
 *
 * @return True if linemate is found on current tile
 */
bool ElevationModule::checkCurrentTileForLinemate() {
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
        return true;
    }

    return false;
}

/**
 * @brief Scan the vision data for tiles containing linemate
 *
 * @return True if a suitable tile with linemate was found
 */
bool ElevationModule::scanVisionForLinemate() {
    for (size_t x = 0; x < AI::Data::i().vision.size(); x++) {
        const size_t midY = AI::Data::i().vision[x].size() / 2;
        for (size_t y = 0; y < AI::Data::i().vision[x].size(); y++) {
            int relativeY = static_cast<int>(y) - static_cast<int>(midY);

            auto& tileContents = AI::Data::i().vision[x][y];
            if (tileContents.find("linemate") != tileContents.end() &&
                tileContents["linemate"] == 1) {
                LOG_INFO("Found linemate at position (%zu,%d)", x, relativeY);
                targetX = static_cast<int>(x);
                targetY = relativeY;
                return true;
            }
        }
    }

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

    if (checkCurrentTileForLinemate()) {
        return true;
    }

    return scanVisionForLinemate();
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
 * @brief Handle the case when linemate was found on a tile
 */
void ElevationModule::handleFoundLinemateTile() {
    if (targetX > 0 || targetY != 0) {
        LOG_INFO("Moving to linemate at (%d,%d)", targetX, targetY);
        AI::Interface::i().goTo(targetX, targetY);
        targetX = 0;
        targetY = 0;
    } else {
        LOG_INFO("Starting incantation");
        AI::Interface::i().sendCommand(INCANTATION);
        foundSpot = false;
        targetX = -1;
        targetY = -1;
    }
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

    if (foundSpot && targetX >= 0 && targetY >= 0) {
        handleFoundLinemateTile();
    }
}
