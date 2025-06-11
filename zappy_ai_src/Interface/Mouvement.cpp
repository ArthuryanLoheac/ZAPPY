#include <string>
#include <vector>
#include <iostream>

#include "Interface/Interface.hpp"
#include "Exceptions/Commands.hpp"

namespace AI {

void Interface::commandFORWARD(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    (void)command;
    if (args.size() != 1) {
        throw AI::CommandArgumentsException("FORWARD",
            "Expected no arguments, got " +
            std::to_string(args.size() - 1));
    }
}

void Interface::commandLEFT(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    (void)command;
    if (args.size() != 1) {
        throw AI::CommandArgumentsException("LEFT",
            "Expected no arguments, got " +
            std::to_string(args.size() - 1));
    }
}

void Interface::commandRIGHT(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    (void)command;
    if (args.size() != 1) {
        throw AI::CommandArgumentsException("RIGHT",
            "Expected no arguments, got " +
            std::to_string(args.size() - 1));
    }
}

}  // namespace AI
