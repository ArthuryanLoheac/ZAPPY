/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Food Gathering Module
*/


#ifndef FOOD_GATHERING_MODULE_HPP
#define FOOD_GATHERING_MODULE_HPP

#include "AIModule.hpp"
#include <map>

/**
 * @brief Module responsible for finding and collecting food
 * 
 * This module scans the environment for food, evaluates the best food sources,
 * and navigates to them while collecting any food found along the way.
 */
class FoodGatheringModule : public AIModule {
public:
    /**
     * @brief Constructor for the FoodGatheringModule
     */
    FoodGatheringModule();

    /**
     * @brief Execute the food gathering behavior
     */
    void execute() override;

    /**
     * @brief Calculate the priority of this module
     * @return float Priority value between 0.0 and 1.0
     */
    float getPriority() override;

    /**
     * @brief Update the internal food count tracker
     * @param count New food count value
     */
    void updateFoodCount(int count);

    /**
     * @brief Update the internal level tracker
     * @param lvl New level value
     */
    void updateLevel(int lvl);

private:
    /**
     * @brief Look around to update vision data
     */
    void findFood();

    /**
     * @brief Check the current tile for food
     * @return bool True if food was found and collected
     */
    bool checkCurrentTileForFood();

    /**
     * @brief Evaluate available food sources to find the best one
     * @param foodLocations Map to store food locations and quantities
     * @return FoodSource Best food source information
     */
    struct FoodSource {
        int x;
        int y;
        float weight;
    };
    FoodSource evaluateFoodSources(std::map<std::pair<int, int>, int>& foodLocations);

    /**
     * @brief Calculate the weight of a food source
     * @param x X coordinate
     * @param y Y coordinate (relative)
     * @param foodCount Amount of food
     * @return float Weight value
     */
    float calculateFoodWeight(size_t x, int relativeY, int foodCount);

    /**
     * @brief Count nearby food around a position
     * @param x X coordinate
     * @param y Y coordinate
     * @return int Number of nearby food items
     */
    int countNearbyFood(size_t x, size_t y);

    /**
     * @brief Make a random move when no good food is found
     */
    void exploreRandomly();

    /**
     * @brief Main function to handle food collection
     */
    void collectFood();

    /**
     * @brief Collect food along a path to the target location
     * @param targetX X coordinate of target
     * @param targetY Y coordinate of target
     * @param foodLocations Map of food locations and quantities
     */
    void collectFoodAlongPath(int targetX, int targetY, 
                            const std::map<std::pair<int, int>, int>& foodLocations);

    /**
     * @brief Move along X axis and collect food along the way
     * @param targetX Target X coordinate
     * @param foodLocations Map of food locations
     * @return int Current X position after moving
     */
    int moveAlongXAxis(int targetX, const std::map<std::pair<int, int>, int>& foodLocations);

    /**
     * @brief Move along Y axis and collect food along the way
     * @param currentX Current X position
     * @param targetY Target Y coordinate
     * @param foodLocations Map of food locations
     */
    void moveAlongYAxis(int currentX, int targetY, const std::map<std::pair<int, int>, int>& foodLocations);

    /**
     * @brief Collect food at a specific location
     * @param x X coordinate
     * @param y Y coordinate
     * @param foodLocations Map of food locations
     */
    void collectFoodAtLocation(int x, int y, const std::map<std::pair<int, int>, int>& foodLocations);
    
    /// Food resource count
    int foodCount = 0;
    
    /// AI level
    int level = 1;
};

#endif // FOOD_GATHERING_MODULE_HPP