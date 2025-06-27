#include "modules/AdvancedLeveler.hpp"

#include <cmath>
#include <unordered_map>
#include <algorithm>

#include "Data/Data.hpp"
#include "Interface/Interface.hpp"
#include "include/logs.h"

const char InvRequestCmd[] = "Gemme you inventory !";
// Add the level of the IAs before the ritual
const char CallingMsg[] = "Wololo !";

static const std::unordered_map<AI::Data::Material_t, double> Rarety = {
    {AI::Data::Linemate, 0.3},
    {AI::Data::Deraumere, 0.15},
    {AI::Data::Sibur, 0.1},
    {AI::Data::Mendiane, 0.1},
    {AI::Data::Phiras, 0.08},
    {AI::Data::Thystame, 0.05}
};

static const std::unordered_map<unsigned int,
    AdvancedLeveler::ElevationRequirements_t>
    ElevationRequirementsMap = {
        {1, {1, {{AI::Data::Linemate, 1}}}},

        {2, {2, {{AI::Data::Linemate, 1},
            {AI::Data::Deraumere, 1},
            {AI::Data::Sibur, 1}}}},

        {3, {2, {{AI::Data::Linemate, 2},
            {AI::Data::Sibur, 1},
            {AI::Data::Phiras, 2}}}},

        {4, {4, {{AI::Data::Linemate, 1},
            {AI::Data::Deraumere, 1},
            {AI::Data::Sibur, 2},
            {AI::Data::Phiras, 1}}}},

        {5, {4, {{AI::Data::Linemate, 1},
            {AI::Data::Deraumere, 2},
            {AI::Data::Sibur, 1},
            {AI::Data::Mendiane, 3}}}},

        {6, {6, {{AI::Data::Linemate, 1},
            {AI::Data::Deraumere, 2},
            {AI::Data::Sibur, 3},
            {AI::Data::Phiras, 1}}}},

        {7, {6, {{AI::Data::Linemate, 2},
            {AI::Data::Deraumere, 2},
            {AI::Data::Sibur, 2},
            {AI::Data::Mendiane, 2},
            {AI::Data::Phiras, 2},
            {AI::Data::Thystame, 1}}}}
};

double AdvancedLeveler::computePriority(int level,
    const std::unordered_map<AI::Data::Material_t, int> &inventory) {
    if (ElevationRequirementsMap.find(level) == ElevationRequirementsMap.end())
        return -1.0;

    const ElevationRequirements_t &requirements
        = ElevationRequirementsMap.at(level);
    double totalRarity = 0.0;
    double missingRarity = 0.0;
    double priority = 0.0;

    for (const auto &[item, required] : requirements.materialsCount) {
        double r = Rarety.count(item) ? Rarety.at(item) : 0.0;
        int have = inventory.count(item) ? inventory.at(item) : 0;
        totalRarity += required * r;
        missingRarity += std::max(0, required - have) * r;
    }
    if (totalRarity + 0.0)
        priority = missingRarity / totalRarity;
    priority *= requirements.playerCount;
    priority = std::log1p(priority) / std::log1p(5.0);
    priority = std::max(1.0, priority);
    return priority;
}

float AdvancedLeveler::getPriority() {
    if (AI::Data::i().level < 2)
        return 1.0f;
    return computePriority(AI::Data::i().level, AI::Data::i().inventory);
}

void AdvancedLeveler::execute() {
    switch (_moduleState) {
        case Idling:
            _moduleState = Listening;
            LOG_DEBUG("Prerequisites met, sending invitation for ritual");
            AI::Interface::i().sendCommand(InvRequestCmd);
            break;
        case Listening:
            break;
        case Calling:
            break;
        case Moving:
            break;
        case Elevating:
            break;
    }
}
