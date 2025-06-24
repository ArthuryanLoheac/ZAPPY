#include "modules/AdvancedLeveler.hpp"

#include <cmath>
#include <unordered_map>
#include <algorithm>

static const std::unordered_map<AdvancedLeveler::Material_t, double> Rarety = {
    {AdvancedLeveler::Linemate, 0.3},
    {AdvancedLeveler::Deraumere, 0.15},
    {AdvancedLeveler::Sibur, 0.1},
    {AdvancedLeveler::Mendiane, 0.1},
    {AdvancedLeveler::Phiras, 0.08},
    {AdvancedLeveler::Thystame, 0.05}
};

static const std::unordered_map<unsigned int,
    AdvancedLeveler::ElevationRequirements_t>
    ElevationRequirementsMap = {
        {1, {1, {{AdvancedLeveler::Linemate, 1}}}},

        {2, {2, {{AdvancedLeveler::Linemate, 1},
            {AdvancedLeveler::Deraumere, 1},
            {AdvancedLeveler::Sibur, 1}}}},

        {3, {2, {{AdvancedLeveler::Linemate, 2},
            {AdvancedLeveler::Sibur, 1},
            {AdvancedLeveler::Phiras, 2}}}},

        {4, {4, {{AdvancedLeveler::Linemate, 1},
            {AdvancedLeveler::Deraumere, 1},
            {AdvancedLeveler::Sibur, 2},
            {AdvancedLeveler::Phiras, 1}}}},

        {5, {4, {{AdvancedLeveler::Linemate, 1},
            {AdvancedLeveler::Deraumere, 2},
            {AdvancedLeveler::Sibur, 1},
            {AdvancedLeveler::Mendiane, 3}}}},

        {6, {6, {{AdvancedLeveler::Linemate, 1},
            {AdvancedLeveler::Deraumere, 2},
            {AdvancedLeveler::Sibur, 3},
            {AdvancedLeveler::Phiras, 1}}}},

        {7, {6, {{AdvancedLeveler::Linemate, 2},
            {AdvancedLeveler::Deraumere, 2},
            {AdvancedLeveler::Sibur, 2},
            {AdvancedLeveler::Mendiane, 2},
            {AdvancedLeveler::Phiras, 2},
            {AdvancedLeveler::Thystame, 1}}}}
};

double AdvancedLeveler::computePriority(int level,
    const std::unordered_map<Material_t, int> &inventory) {
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
