/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Food Gathering Module Implementation
*/

#include "modules/FoodGatheringModule.hpp"
#include <iostream>
#include <random>
#include <map>
#include "../Logic/AIBase.hpp"
#include "../Interface/Interface.hpp"
#include "../Exceptions/Factory.hpp"
#include "../Exceptions/Commands.hpp"
#include "../Data/Data.hpp"
#include "../../libc/include/logs.h"

/**
 * @brief Initialize the FoodGatheringModule with default values
 */
FoodGatheringModule::FoodGatheringModule() {
    foodCount = 10;
    level = 1;
}

/**
 * @brief Execute the food gathering behavior
 *
 * Periodically checks inventory and handles the food gathering process
 */
void FoodGatheringModule::execute() {
    foodCount = AI::Data::i().inventory.find("food") !=
                AI::Data::i().inventory.end() ?
                AI::Data::i().inventory.at("food") : 0;
    level = AI::Data::i().level;
    findFood();
    collectFood();
}

/**
 * @brief Calculate the priority of this module
 *
 * Priority is based on the food count and AI level
 *
 * @return float Priority value between 0.0 and 1.0
 */
float FoodGatheringModule::getPriority() {
    foodCount = AI::Data::i().inventory.find("food") !=
                AI::Data::i().inventory.end() ?
                AI::Data::i().inventory.at("food") : 0;
    float prio = (static_cast<float>(foodCount) / 10.0f) -
                 (static_cast<float>(level) / 8.0f);
    if (prio < 0.0f) prio = 0.0f;
    if (prio > 1.0f) prio = 1.0f;
    std::cout << "Food Gathering Module Priority: " << prio
              << "with food count: " << foodCount
              << " and level: " << level << std::endl;
    return prio;
}

/**
 * @brief Update the internal food count tracker
 * @param count New food count value
 */
void FoodGatheringModule::updateFoodCount(int count) {
    foodCount = count;
}

/**
 * @brief Update the internal level tracker
 * @param lvl New level value
 */
void FoodGatheringModule::updateLevel(int lvl) {
    level = lvl;
}

/**
 * @brief Look around to update vision data
 */
void FoodGatheringModule::findFood() {
    AI::Interface::i().sendCommand(LOOK);
}

/**
 * @brief Check the current tile for food
 * @return bool True if food was found and collected
 */
bool FoodGatheringModule::checkCurrentTileForFood() {
    if (!AI::Data::i().vision.empty() &&
        !AI::Data::i().vision[0].empty() &&
        AI::Data::i().vision[0].size() >= 3) {
        const size_t midY = AI::Data::i().vision[0].size() / 2;
        auto& currentTile = AI::Data::i().vision[0][midY];

        if (currentTile.find("food") != currentTile.end() &&
            currentTile["food"] > 0) {
            std::cout << "Food found on current tile: "
                      << currentTile["food"] << std::endl;

            for (int i = 0; i < currentTile["food"]; i++) {
                AI::Interface::i().sendCommand("Take food\n");
            }

            return true;
        } else {
            std::cout << "No food on current tile." << std::endl;
        }
    }
    return false;
}

/**
 * @brief Count nearby food around a position
 * @param x X coordinate
 * @param y Y coordinate
 * @return int Number of nearby food items
 */
int FoodGatheringModule::countNearbyFood(size_t x, size_t y) {
    int nearbyFood = 0;

    if (x > 0 && x < AI::Data::i().vision.size() - 1) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if ((dx == 0 && dy == 0) ||
                    (static_cast<int>(y) + dy < 0) ||
                    (y + dy >= AI::Data::i().vision[x+dx].size()))
                    continue;

                auto& adjacentTile = AI::Data::i().vision[x+dx][y+dy];
                if (adjacentTile.find("food") != adjacentTile.end()) {
                    nearbyFood += adjacentTile["food"];
                }
            }
        }
    }

    return nearbyFood;
}

/**
 * @brief Calculate the weight of a food source
 * @param x X coordinate
 * @param relativeY Y coordinate (relative)
 * @param foodCount Amount of food
 * @return float Weight value
 */
float FoodGatheringModule::calculateFoodWeight(size_t x, int relativeY,
                                             int foodCount) {
    const float FOOD_WEIGHT = 5.0f;
    const float FOOD_BONUS = 3.0f;
    const float DISTANCE_PENALTY = 1.0f;
    const float DIRECTION_PENALTY = 2.0f;
    const float NEARBY_FOOD_BONUS = 1.0f;
    const float CURRENT_TILE_BONUS = 10.0f;

    float weight = FOOD_WEIGHT;

    // Special case: current tile (0,0) should be HEAVILY prioritized
    if (x == 0 && relativeY == 0) {
        weight += CURRENT_TILE_BONUS;
        std::cout << "Adding special weight for food on current tile!"
                  << std::endl;
    }

    // Bonus for additional food on the same tile
    if (foodCount > 1) {
        weight += (foodCount - 1) * FOOD_BONUS;
    }

    // Bonus for nearby food
    int nearbyFood = countNearbyFood(x, x);
    weight += nearbyFood * NEARBY_FOOD_BONUS;

    // Distance penalty
    weight -= static_cast<float>(x) * DISTANCE_PENALTY;

    // Direction penalty (not being in the middle column)
    if (relativeY != 0) {
        weight -= DIRECTION_PENALTY;
    }

    return weight;
}

/**
 * @brief Evaluate available food sources to find the best one
 * @param foodLocations Map to store food locations and quantities
 * @return FoodSource Best food source information
 */
FoodGatheringModule::FoodSource FoodGatheringModule::evaluateFoodSources(
    std::map<std::pair<int, int>, int>& foodLocations) {

    FoodSource bestSource = {0, 0, -1};

    for (size_t x = 0; x < AI::Data::i().vision.size(); x++) {
        const size_t midY = AI::Data::i().vision[x].size() / 2;
        for (size_t y = 0; y < AI::Data::i().vision[x].size(); y++) {
            int relativeY = static_cast<int>(y) - static_cast<int>(midY);

            auto& tileContents = AI::Data::i().vision[x][y];
            if (tileContents.find("food") != tileContents.end() &&
                tileContents["food"] > 0) {
                int foodCount = tileContents["food"];

                foodLocations[{static_cast<int>(x), relativeY}] = foodCount;

                float weight = calculateFoodWeight(x, relativeY, foodCount);

                if (weight > bestSource.weight) {
                    bestSource.x = static_cast<int>(x);
                    bestSource.y = relativeY;
                    bestSource.weight = weight;
                }

                std::cout << "Food at (" << x << "," << relativeY << "): "
                          << foodCount << " items, weight = " << weight
                          << std::endl;
            }
        }
    }

    return bestSource;
}

/**
 * @brief Make a random move when no good food is found
 */
void FoodGatheringModule::exploreRandomly() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1, 100);

    int randomChoice = dis(gen);

    // 40% chance to turn left (1-40)
    // 35% chance to turn right (41-75)
    // 25% chance to go forward (76-100)
    // (add chance to spawn egg + leech if food count is high)
    if (randomChoice <= 40) {
        AI::Interface::i().sendCommand(LEFT);
    } else if (randomChoice <= 75) {
        AI::Interface::i().sendCommand(RIGHT);
    } else {
        AI::Interface::i().sendCommand(FORWARD);
    }
}

/**
 * @brief Collect food at a specific location
 * @param x X coordinate
 * @param y Y coordinate
 * @param foodLocations Map of food locations
 */
void FoodGatheringModule::collectFoodAtLocation(int x, int y,
    const std::map<std::pair<int, int>, int>& foodLocations) {
    auto it = foodLocations.find({x, y});
    if (it != foodLocations.end()) {
        for (int i = 0; i < it->second; i++) {
            AI::Interface::i().sendCommand("Take food\n");
        }
    }
}

/**
 * @brief Move along X axis and collect food along the way
 * @param targetX Target X coordinate
 * @param foodLocations Map of food locations
 * @return int Current X position after moving
 */
int FoodGatheringModule::moveAlongXAxis(int targetX,
    const std::map<std::pair<int, int>, int>& foodLocations) {
    int currentX = 0;
    int currentY = 0;

    for (int x = 1; x <= targetX; x++) {
        AI::Interface::i().sendCommand(FORWARD);
        currentX = x;

        collectFoodAtLocation(currentX, currentY, foodLocations);
    }

    return currentX;
}

/**
 * @brief Move along Y axis and collect food along the way
 * @param currentX Current X position
 * @param targetY Target Y coordinate
 * @param foodLocations Map of food locations
 */
void FoodGatheringModule::moveAlongYAxis(int currentX, int targetY,
    const std::map<std::pair<int, int>, int>& foodLocations) {
    int currentY = 0;

    if (targetY < 0) {  // Need to move left
        AI::Interface::i().sendCommand(LEFT);
        for (int y = -1; y >= targetY; y--) {
            AI::Interface::i().sendCommand(FORWARD);
            currentY = y;

            collectFoodAtLocation(currentX, currentY, foodLocations);
        }
    } else if (targetY > 0) {  // Need to move right
        AI::Interface::i().sendCommand(RIGHT);
        for (int y = 1; y <= targetY; y++) {
            AI::Interface::i().sendCommand(FORWARD);
            currentY = y;

            collectFoodAtLocation(currentX, currentY, foodLocations);
        }
    }
}

/**
 * @brief Main function to handle food collection
 */
void FoodGatheringModule::collectFood() {
    if (AI::Data::i().vision.empty()) {
        return;
    }

    // First check the current tile for food
    if (checkCurrentTileForFood()) {
        return;
    }

    // Map to track all food locations (x, y) -> food count
    std::map<std::pair<int, int>, int> foodLocations;

    // Find the best food source
    const float WEIGHT_THRESHOLD = 3.0f;
    FoodSource bestSource = evaluateFoodSources(foodLocations);

    // If we found food with sufficient weight, collect all food along the path
    if (bestSource.weight >= WEIGHT_THRESHOLD) {
        std::cout << "Best food source at (" << bestSource.x << ","
                  << bestSource.y << ") with weight " << bestSource.weight
                  << std::endl;
        collectFoodAlongPath(bestSource.x, bestSource.y, foodLocations);
    } else {
        exploreRandomly();
    }
}

/**
 * @brief Collect food along a path to the target location
 * @param targetX X coordinate of target
 * @param targetY Y coordinate of target
 * @param foodLocations Map of food locations and quantities
 */
void FoodGatheringModule::collectFoodAlongPath(int targetX, int targetY,
    const std::map<std::pair<int, int>, int>& foodLocations) {
    // Move along X axis first
    int currentX = moveAlongXAxis(targetX, foodLocations);

    // Then handle Y direction if needed
    if (targetY != 0) {
        moveAlongYAxis(currentX, targetY, foodLocations);
    }

    // Final check for food at the destination
    collectFoodAtLocation(targetX, targetY, foodLocations);
}
