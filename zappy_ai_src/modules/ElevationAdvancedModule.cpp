#include "modules/ElevationAdvancedModule.hpp"
#include <iostream>
#include <random>
#include <map>
#include "../Logic/AIBase.hpp"
#include "../Interface/Interface.hpp"
#include "../Exceptions/Factory.hpp"
#include "../Exceptions/Commands.hpp"
#include "../Data/Data.hpp"

void ElevationAdvancedModule::execute() {
    inventoryCheckCount++;

    foodCount = AI::Data::i().inventory.find(AI::Data::Food) !=
                AI::Data::i().inventory.end() ?
                AI::Data::i().inventory.at(AI::Data::Food) : 0;
    level = AI::Data::i().level;

    if (inventoryCheckCount % 10 == 0) {
        AI::Interface::i().sendCommand(INVENTORY);
    }
    printf("Food count: %d, Level: %d\n", foodCount, level);
}

float ElevationAdvancedModule::getPriority() {
    return 1.0f;
}
