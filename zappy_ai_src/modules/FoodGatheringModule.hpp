/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/


#ifndef FOOD_GATHERING_MODULE_HPP
#define FOOD_GATHERING_MODULE_HPP

#include "AIModule.hpp"

class FoodGatheringModule : public AIModule {
public:
    FoodGatheringModule();
    std::string execute(const GameData& data) override;
    float getPriority() const override;

    void updateFoodCount(int count);
    void updateLevel(int lvl);

private:
    void findFood();
    void collectFood();

    int foodCount = 0;
    int level = 1;
};

#endif // FOOD_GATHERING_MODULE_HPP