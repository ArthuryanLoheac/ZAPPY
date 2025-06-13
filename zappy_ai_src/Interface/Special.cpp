#include <unistd.h>

#include <string>
#include <vector>
#include <iostream>
#include <csignal>

#include "Interface/Interface.hpp"
#include "Exceptions/Commands.hpp"

namespace AI {

void Interface::commandFORK(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    if (args.size() != 1) {
        throw AI::CommandArgumentsException("FORK",
            "Expected no arguments, got " +
            std::to_string(args.size() - 1));
    }
    (void)command;

    if (args[0] == "OK") {
        kill(getppid(), SIGUSR1);
    }
}

}  // namespace AI
