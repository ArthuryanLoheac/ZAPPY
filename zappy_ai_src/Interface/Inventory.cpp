#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "Interface/Interface.hpp"
#include "Exceptions/Commands.hpp"
#include "Data/Data.hpp"

namespace AI {

void Interface::commandLOOK(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    (void)command;
    if (args.size() < 2) {
        throw CommandArgumentsException("LOOK",
            "Expected at least one argument, got " +
            std::to_string(args.size() - 1));
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

void Interface::commandINVENTORY(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    (void)command;

    if (args.size() < 2) {
        throw CommandArgumentsException("INVENTORY",
            "Expected at least one argument, got " +
            std::to_string(args.size() - 1));
    }

    if (args[0] == "[")
        args.erase(args.begin());

    if (args.back() == "]")
        args.pop_back();

    Data::i().inventory.clear();
    for (size_t i = 0; i < args.size(); i += 2) {
        if (i + 1 >= args.size()) {
            throw CommandArgumentsException("INVENTORY",
                "Expected an even number of arguments, got " +
                std::to_string(args.size() - 1));
        }
        const std::string &item = args[i];
        int quantity = std::stoi(args[i + 1]);
        Data::i().inventory[item] += quantity;
    }
}

void Interface::commandTAKE(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    if (args.size() != 1) {
        throw CommandArgumentsException("TAKE",
            "Expected one argument, got " +
            std::to_string(args.size()));
    }

    const std::string &status = args[0];

    if (status == "KO") return;

    Data::i().inventory[command[1]]++;
}

void Interface::commandSET(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    if (args.size() != 1) {
        throw CommandArgumentsException("SET",
            "Expected one argument, got " +
            std::to_string(args.size()));
    }

    const std::string &status = args[0];

    if (status == "KO") return;

    Data::i().inventory[command[1]]--;
}

}  // namespace AI
