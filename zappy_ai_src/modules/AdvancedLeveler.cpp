#include "modules/AdvancedLeveler.hpp"

#include <unistd.h>

#include <cmath>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <format>
#include <tuple>
#include <regex>
#include <vector>
#include <utility>
#include <iostream>

#include "Data/Data.hpp"
#include "Interface/Interface.hpp"
#include "include/logs.h"

const char InvRequestCmd[] = "Gemme you inventory !";
// Add the level of the IAs before the ritual
const char CallingMsg[] = "Wololo !";

static const std::unordered_map<AI::Data::Material_t, double> Rarety = {
    {AI::Data::Material_t::Linemate, 0.3},
    {AI::Data::Material_t::Deraumere, 0.15},
    {AI::Data::Material_t::Sibur, 0.1},
    {AI::Data::Material_t::Mendiane, 0.1},
    {AI::Data::Material_t::Phiras, 0.08},
    {AI::Data::Material_t::Thystame, 0.05}
};

/**
 * @brief All elevations material requirements
 * Map of AdvancedLeveler::ElevationRequirements_t by their levels.
 */
static const std::unordered_map<unsigned int,
    AdvancedLeveler::ElevationRequirements_t>
    ElevationRequirementsMap = {
        {1, {1, {{AI::Data::Material_t::Linemate, 1}}}},

        {2, {2, {{AI::Data::Material_t::Linemate, 1},
            {AI::Data::Material_t::Deraumere, 1},
            {AI::Data::Material_t::Sibur, 1}}}},

        {3, {2, {{AI::Data::Material_t::Linemate, 2},
            {AI::Data::Material_t::Sibur, 1},
            {AI::Data::Material_t::Phiras, 2}}}},

        {4, {4, {{AI::Data::Material_t::Linemate, 1},
            {AI::Data::Material_t::Deraumere, 1},
            {AI::Data::Material_t::Sibur, 2},
            {AI::Data::Material_t::Phiras, 1}}}},

        {5, {4, {{AI::Data::Material_t::Linemate, 1},
            {AI::Data::Material_t::Deraumere, 2},
            {AI::Data::Material_t::Sibur, 1},
            {AI::Data::Material_t::Mendiane, 3}}}},

        {6, {6, {{AI::Data::Material_t::Linemate, 1},
            {AI::Data::Material_t::Deraumere, 2},
            {AI::Data::Material_t::Sibur, 3},
            {AI::Data::Material_t::Phiras, 1}}}},

        {7, {6, {{AI::Data::Material_t::Linemate, 2},
            {AI::Data::Material_t::Deraumere, 2},
            {AI::Data::Material_t::Sibur, 2},
            {AI::Data::Material_t::Mendiane, 2},
            {AI::Data::Material_t::Phiras, 2},
            {AI::Data::Material_t::Thystame, 1}}}}
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
    std::cout << "Computed priority: " << priority
              << " for level: " << level << std::endl;
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
        AI::Data::i().inventory.at(AI::Data::Material_t::Linemate),
        AI::Data::i().inventory.at(AI::Data::Material_t::Deraumere),
        AI::Data::i().inventory.at(AI::Data::Material_t::Sibur),
        AI::Data::i().inventory.at(AI::Data::Material_t::Mendiane),
        AI::Data::i().inventory.at(AI::Data::Material_t::Phiras),
        AI::Data::i().inventory.at(AI::Data::Material_t::Thystame));
}

/**
 * Message used in errors logs when the parsing of an inventory content msg fails
 */
static const char defaultInvContentMsgForErr[] ="InvContent[id] "
    "Linemate,Deraumere,Sibur,Mendiane,Phiras,Thystame";

/**
 * @return Optional pair with id;AI::Data::Inventory_t
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
    inv[AI::Data::Material_t::Linemate] = parsedInv[0];
    inv[AI::Data::Material_t::Deraumere] = parsedInv[0];
    inv[AI::Data::Material_t::Sibur] = parsedInv[0];
    inv[AI::Data::Material_t::Mendiane] = parsedInv[0];
    inv[AI::Data::Material_t::Phiras] = parsedInv[0];
    inv[AI::Data::Material_t::Thystame] = parsedInv[0];

    return std::make_pair(id, inv);
}

/**
 * @brief Checks if the required materials for the required elevation ritual are
 * located on the AI's current tile.
 */
static bool isRequiredMaterialsOnGround(int ritualLevel) {
    const auto &tile = AI::Data::i().vision.at(0).at(0);
    const AdvancedLeveler::ElevationRequirements_t &requiredMaterials =
        ElevationRequirementsMap.at(ritualLevel);

    for (const auto &[material, amount] : requiredMaterials.materialsCount) {
        if (tile.count(AI::Data::materialToString(material)) == 0)
            return false;
        if (tile.at(AI::Data::materialToString(material)) >= amount)
            return true;
    }
    return false;
}

/**
 * @brief Checks if the required materials for the required elevation ritual are
 * located in the inventory.
 */
static bool isRequiredMaterialsInInv(AI::Data::Inventory_t &inv,
int ritualLevel) {
    const AdvancedLeveler::ElevationRequirements_t &requiredMaterials =
        ElevationRequirementsMap.at(ritualLevel);

    for (const auto &[material, amount] : requiredMaterials.materialsCount) {
        if (inv.count(material) == 0)
            return false;
        if (inv.at(material) >= amount)
            return true;
    }
    return true;
}

/**
 * @brief Spits one material required for the required elevation ritual based
 * on actual items on the ground.
 *
 * If all items are already present on the ground, does nothing.
 */
static void spitRequiredMaterial(AI::Data::Inventory_t &inv, int ritualLevel) {
    const AdvancedLeveler::ElevationRequirements_t &requiredMaterials =
        ElevationRequirementsMap.at(ritualLevel);

    for (const auto &[material, amount] : requiredMaterials.materialsCount) {
        if ((inv.count(material) == 0 || inv.at(material) < amount)
            && inv.count(material) > 0) {
            AI::Interface::i().sendCommand(std::format("SET {}",
                AI::Data::materialToString(material)));
            return;
            }
    }
}

float AdvancedLeveler::getPriority() {
    static int inventoryPrintCounter = 0;
    if (++inventoryPrintCounter >= 10) {
        inventoryPrintCounter = 0;
        std::cout << "Inventory: ";
        for (const auto& [mat, qty] : AI::Data::i().inventory) {
            std::cout << AI::Data::materialToString(mat) << "=" << qty << " ";
        }
        std::cout << std::endl;
    }
    auto &msgQueue = AI::Data::i().messageQueue;

    if (AI::Data::i().level < 2)
        return 1.0f;
    if (!msgQueue.empty() && msgQueue.front().first == InvRequestCmd
        && !_isCaller) {
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
    if (!msgQueue.empty()
        && msgQueue.front().first == std::string(CallingMsg)
        && !msgQueue.front().second) {
        _moduleState = Moving;
        return 0.3f;
    }

    return computePriority(AI::Data::i().level, AI::Data::i().inventory);
}

void AdvancedLeveler::execute() {
    std::cout << "Player with PID " << getpid()
              << " executing Advanced Leveler Module with state: "
              << static_cast<int>(_moduleState) << std::endl;
    switch (_moduleState) {
        case Idling: {
            LOG_INFO("Prerequisites met, sending invitation for ritual");
            AI::Interface::i().sendCommand(InvRequestCmd);
            _moduleState = Listening;
        }
            break;

        case Answering: {
            AI::Interface::i().sendCommand(getInvString());
            _moduleState = Idling;
        }
            break;

        case Listening: {
            _isCaller = true;
            auto &queue = AI::Data::i().messageQueue;

            if (!queue.empty()) {
                const auto &[msg, direction] = queue.front();
                auto parsed = parseInventoryMessage(msg);

                if (parsed != std::nullopt) {
                    int id = std::get<0>(parsed.value());
                    AI::Data::Inventory_t inv = std::get<1>(parsed.value());
                    _othersInv[id] = inv;
                    queue.pop();
                }
            }
            if (_othersInv.size() >= 5) {
                AI::Data::Inventory_t totalInv = AI::Data::i().inventory;
                for (const auto &inv : _othersInv) {
                    for (const auto &[material, count] : inv.second) {
                        totalInv[material] += count;
                    }
                }
                if (isRequiredMaterialsInInv(totalInv, AI::Data::i().level)) {
                    _moduleState = Calling;
                } else {
                    _isCaller = false;
                    _moduleState = Idling;
                }
            }
        }
            break;

        case Calling: {
            if (AI::Data::i().vision.at(0).at(0).count("player")
                && AI::Data::i().vision.at(0).at(0).at("player") >= 6) {
                if (isRequiredMaterialsOnGround(AI::Data::i().level)) {
                    _moduleState = Spitting;
                }
                break;
            } else {
                AI::Interface::i().sendCommand(CallingMsg);
            }
        }
            break;

        case Moving: {
            if (AI::Data::i().messageQueue.empty())
                break;
            if (AI::Data::i().messageQueue.front().second == 0)
                _moduleState = Spitting;
            AI::Interface::i().goToDirection(
                AI::Data::i().messageQueue.front().second);
            AI::Data::i().messageQueue.pop();
        }
            break;

        case Spitting: {
            if (isRequiredMaterialsOnGround(AI::Data::i().level)) {
                if (_isCaller)
                    AI::Interface::i().sendCommand(INCANTATION);
                _moduleState = Elevating;
            } else {
                spitRequiredMaterial(AI::Data::i().inventory,
                    AI::Data::i().level);
            }
        }
            break;

        case Elevating:
            // Dunno
            break;
    }
}
