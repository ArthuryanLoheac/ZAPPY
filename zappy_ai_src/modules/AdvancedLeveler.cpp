#include "modules/AdvancedLeveler.hpp"

#include <cmath>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <format>
#include <tuple>
#include <regex>
#include <vector>
#include <utility>

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

/**
 * This function is temporary.
 */
static int getId() {
    return 10;
}

static std::string getInvString() {
    return std::format("InvContent[{}] {},{},{},{},{},{}", getId(),
        AI::Data::i().inventory.at(AI::Data::Linemate),
        AI::Data::i().inventory.at(AI::Data::Deraumere),
        AI::Data::i().inventory.at(AI::Data::Sibur),
        AI::Data::i().inventory.at(AI::Data::Mendiane),
        AI::Data::i().inventory.at(AI::Data::Phiras),
        AI::Data::i().inventory.at(AI::Data::Thystame));
}

static const char defaultInvContentMsgForErr[] ="InvContent[id] "
    "Linemate,Deraumere,Sibur,Mendiane,Phiras,Thystame";
/**
 * @return Optional pair with id;inventory
 */
static std::optional<std::pair<int, AI::Data::Inventory_t>>
parseInventoryMessage(const std::string &msg) {
    std::regex pattern(R"(InvContent\[(\d+)\]\s+(\d+,\d+,\d+,\d+,\d+,\d+))");
    std::smatch match;
    int id;

    if (!std::regex_match(msg, match, pattern) || match.size() != 3)
        return std::nullopt;
    try {
        id = std::stoi(match[1].str());
    } catch (...) {
        LOG_ERROR("Failed to parse invContentStr id:"
            "got '%s' but expected '%s'", msg, defaultInvContentMsgForErr);
        return std::nullopt;
    }

    std::string inventoryStr = match[2].str();
    std::istringstream ss(inventoryStr);
    std::string token;
    std::vector<int> parsedInv;

    while (std::getline(ss, token, ',')) {
        try {
            parsedInv.push_back(std::stoi(token));
        } catch (...) {
            LOG_ERROR("Failed to parse invContentStr itemAmount:"
                "got '%s' but expected '%s'", msg, defaultInvContentMsgForErr);
            return std::nullopt;
        }
    }
    if (parsedInv.size() != 6) {
        LOG_ERROR("Failed to parse invContentStr wrong ressources type count:"
            "got '%s' but expected '%s'", msg, defaultInvContentMsgForErr);
        return std::nullopt;
    }

    AI::Data::Inventory_t inv;
    inv[AI::Data::Linemate] = parsedInv[0];
    inv[AI::Data::Deraumere] = parsedInv[0];
    inv[AI::Data::Sibur] = parsedInv[0];
    inv[AI::Data::Mendiane] = parsedInv[0];
    inv[AI::Data::Phiras] = parsedInv[0];
    inv[AI::Data::Thystame] = parsedInv[0];

    return std::make_pair(id, inv);
}

float AdvancedLeveler::getPriority() {
    auto &msgQueue = AI::Data::i().messageQueue;

    if (AI::Data::i().level < 2)
        return 1.0f;
    if (!msgQueue.empty() && msgQueue.front().first == InvRequestCmd) {
        _moduleState = Answering;
        return 0.0f;
    }
    if (_moduleState == Listening) {
        if (!msgQueue.empty()
            && parseInventoryMessage(msgQueue.front().first) != std::nullopt) {
            return 0.0f;
        } else {
            return 1.0f;
        }
    }
    return computePriority(AI::Data::i().level, AI::Data::i().inventory);
}

void AdvancedLeveler::execute() {
    switch (_moduleState) {
        case Idling:
            LOG_INFO("Prerequisites met, sending invitation for ritual");
            AI::Interface::i().sendCommand(InvRequestCmd);
            _moduleState = Listening;
            break;
        case Answering:
            AI::Interface::i().sendCommand(getInvString());
            _moduleState = Idling;
            break;
        case Listening: {
            auto &queue = AI::Data::i().messageQueue;

            if (!queue.empty()) {
                const auto &[msg, direction] = queue.front();
                auto parsed = parseInventoryMessage(msg);

                if (parsed != std::nullopt) {
                    int id = std::get<0>(parsed.value());
                    std::pair<int, AI::Data::Inventory_t> inv = {
                        direction, std::get<1>(parsed.value())};
                    _othersInv[id] = inv;
                    queue.pop();
                }
            }
        }
            break;
        case Calling:
            break;
        case Moving:
            break;
        case Elevating:
            break;
    }
}
