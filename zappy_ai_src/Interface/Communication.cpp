#include <string>
#include <vector>
#include <iostream>

#include "Interface/Interface.hpp"
#include "Exceptions/Commands.hpp"
#include "Data/Data.hpp"

namespace AI {

void Interface::commandBROADCAST(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    if (args.size() < 2) {
        throw AI::CommandArgumentsException("BROADCAST",
            "Expected at least one argument, got " +
            std::to_string(args.size() - 1));
    }
    (void)command;
}

void Interface::receiveMessage(std::vector<std::string> &args) {
    if (args.size() != 3) {
        throw AI::CommandArgumentsException("MESSAGE",
            "Expected 3 arguments, got " +
            std::to_string(args.size() - 1));
    }

    std::string message = args[1];
    int direction = std::stoi(args[2]);

    if (needFilter(message)) {
        return;
    }

    Data::i().messageQueue.push({message, direction});
}

void Interface::initializeFilteredStrings() {
    filteredStrings = {
        "Salut c'est Frank Leboeuf, vous voulez vendre votre voiture ?",
        ""
    };
}

// This function filters out "Crazy of the bus" random crap
bool Interface::needFilter(const std::string &arg) {
    if (std::find(filteredStrings.begin(), filteredStrings.end(), arg) !=
        filteredStrings.end()) {
        return true;
    }

    if (!arg.starts_with(Data::i().magicKey))
        return true;

    return false;
}

}  // namespace AI
