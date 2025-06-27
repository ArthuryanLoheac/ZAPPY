#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "Interface/Interface.hpp"
#include "Exceptions/Commands.hpp"
#include "Data/Data.hpp"
#include "include/logs.h"

/**
 * @file Inventory.cpp
 * @brief Implementation of inventory and perception-related command handlers
 *
 * This file contains the implementation of commands that deal with
 * the AI's inventory management and perception of the environment.
 */

namespace AI {

/**
 * @brief Handles the LOOK command response
 *
 * Processes the response from the server after requesting to look around.
 * Parses the vision data into the AI's structured vision representation.
 *
 * @param args Server response arguments containing vision data
 * @param command Original command sent
 */
void Interface::commandLOOK(std::vector<std::string> &args,
std::vector<std::string> &command) {
    (void)command;
    if (args.size() < 2) {
        LOG_WARNING("LOOK: Expected at least one argument, got %i\n.",
            args.size() - 1);
        return;
    }

    if (args[0] == "[")
        args.erase(args.begin());

    if (args.back() == "]")
        args.pop_back();

    std::vector<std::unordered_map<std::string, int>> parsedArgs;
    std::unordered_map<std::string, int> current;

    for (const auto &arg : args) {
        if (arg.find(',') != std::string::npos) {
            std::string remaining = arg;
            size_t pos = 0;
            while ((pos = remaining.find(',')) != std::string::npos) {
                std::string part = remaining.substr(0, pos);
                if (!part.empty()) {
                    current[part]++;
                }
                parsedArgs.push_back(current);
                current.clear();
                remaining = remaining.substr(pos + 1);
            }
            if (!remaining.empty()) {
                current[remaining]++;
            }
        } else if (arg == ",") {
            parsedArgs.push_back(current);
            current.clear();
        } else {
            current[arg]++;
        }
    }

    parsedArgs.push_back(current);

    Data::i().vision.clear();
    int x = 0;
    int y = 0;

    for (const auto &map : parsedArgs) {
        if (y == (x * 2 + 1)) {
            y = 0;
            x++;
        }
        if (x >= static_cast<int>(Data::i().vision.size())) {
            Data::i().vision.emplace_back();
        }
        if (y >= static_cast<int>(Data::i().vision[x].size())) {
            Data::i().vision[x].emplace_back();
        }
        for (const auto &pair : map) {
            Data::i().vision[x][y][pair.first] += pair.second;
        }
        y++;
    }
}

/**
 * @brief Handles the INVENTORY command response
 *
 * Processes the response from the server after requesting inventory information.
 * Parses the inventory data and updates the AI's inventory representation.
 *
 * @param args Server response arguments containing inventory data
 * @param command Original command sent
 */
void Interface::commandINVENTORY(std::vector<std::string> &args,
std::vector<std::string> &command) {
    (void)command;
    if (args.size() < 2) {
        LOG_WARNING("INVENTORY: Expected at least one argument, got %i\n.",
            args.size() - 1);
        return;
    }

    if (args[0] == "[")
        args.erase(args.begin());

    if (args.back() == "]")
        args.pop_back();

    Data::i().inventory.clear();
    for (size_t i = 0; i < args.size(); i += 2) {
        if (i + 1 >= args.size()) {
            LOG_WARNING("INVENTORY: Expected an even number of arguments,"
                "got %i\n.", args.size() - 1);
            return;
        }
        const std::string &item = args[i];
        int quantity = std::stoi(args[i + 1]);
        Data::i().inventory[Data::stringToMaterial.at(item)] += quantity;
    }
}

void Interface::commandTAKE(std::vector<std::string> &args,
std::vector<std::string> &command) {
    if (args.size() != 1) {
        LOG_WARNING("TAKE: Expected one argument, got %i\n.",
            args.size() - 1);
        return;
    }

    const std::string &status = args[0];

    if (status == "KO") return;

    Data::i().inventory[Data::stringToMaterial.at(command[0])]++;
}

void Interface::commandSET(std::vector<std::string> &args,
std::vector<std::string> &command) {
    if (args.size() != 1) {
        LOG_WARNING("SET: Expected one argument, got %i\n.",
            args.size() - 1);
        return;
    }

    const std::string &status = args[0];

    if (status == "KO") return;

    Data::i().inventory[Data::stringToMaterial.at(command[0])]--;
}

}  // namespace AI
