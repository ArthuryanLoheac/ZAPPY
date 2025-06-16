/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/

#include "FoodGatheringModule.hpp"
#include "AIBase.hpp"
#include <iostream>
#include "../Interface/Interface.hpp"
#include "../Exceptions/Factory.hpp"
#include "../Exceptions/Commands.hpp"
#include "../Data/Data.hpp"

FoodGatheringModule::FoodGatheringModule() {
    foodCount = 10;
    level = 1;
}

void FoodGatheringModule::execute() {
    static int inventoryCheckCount = 0;
    inventoryCheckCount++;

    foodCount = AI::Data::i().inventory.find("food") != AI::Data::i().inventory.end() ? 
                AI::Data::i().inventory.at("food") : 0;
    level = AI::Data::i().level;
    
    if (inventoryCheckCount % 10 == 0) {
        std::cout << "Checking inventory..." << std::endl;
        AI::Interface::i().sendCommand(INVENTORY);
    } else {
        std::cout << "Current food: " << foodCount << ", level: " << level << std::endl;
        AI::Interface::i().sendCommand(FORWARD);
    }
}

float FoodGatheringModule::getPriority() {
    foodCount = AI::Data::i().inventory.find("food") != AI::Data::i().inventory.end() ?
            AI::Data::i().inventory.at("food") : 0;
    float prio = (static_cast<float>(foodCount) / 10.0f) - (static_cast<float>(level) / 8.0f);
    if (prio < 0.0f) prio = 0.0f;
    if (prio > 1.0f) prio = 1.0f;
    std::cout << "Food Gathering Module Priority: " << prio << std::endl;
    return prio;
}

void FoodGatheringModule::updateFoodCount(int count) {
    foodCount = count;
}

void FoodGatheringModule::updateLevel(int lvl) {
    level = lvl;
}