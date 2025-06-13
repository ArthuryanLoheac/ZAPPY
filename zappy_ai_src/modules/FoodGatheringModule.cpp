/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/

#include "FoodGatheringModule.hpp"
#include "AIBase.hpp"
#include <iostream>

FoodGatheringModule::FoodGatheringModule() {
    // Constructor implementation (if needed)
    foodCount = 10;
    level = 1;
}

std::string FoodGatheringModule::execute(const GameData& data) {
    static int inventoryCheckCount = 0;
    inventoryCheckCount++;
    if (inventoryCheckCount % 10 == 0) {
        std::cout << "Checking inventory..." << std::endl;
        return "Inventory\n";
    }
    // Update local state from game data
    foodCount = data.inventory.find("Food") != data.inventory.end() ? data.inventory.at("Food") : 0;
    level = data.level;

    std::cout << "Current food: " << foodCount << ", level: " << level << std::endl;

    return "Forward\n";
}

float FoodGatheringModule::getPriority() const {
    float prio = (static_cast<float>(foodCount) / 10.0f) - (static_cast<float>(level) / 8.0f);
    if (prio < 0.0f) prio = 0.0f;
    if (prio > 1.0f) prio = 1.0f;
    //std::cout << "Food Gathering Module Priority: " << prio << std::endl;
    return prio;
}

void FoodGatheringModule::updateFoodCount(int count) {
    foodCount = count;
}

void FoodGatheringModule::updateLevel(int lvl) {
    level = lvl;
}