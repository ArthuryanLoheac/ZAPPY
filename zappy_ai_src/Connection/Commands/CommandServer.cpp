#include <iostream>
#include <string>
#include <vector>

#include "Connection/ServerAI.hpp"
#include "Exceptions/ServerAIExceptions.hpp"
#include "DataManager/DataManager.hpp"

void printError(const std::exception &e, std::vector<std::string> &args) {
    std::cerr << "Error: " << e.what() << std::endl;
    std::cerr << "Args: ";
    for (const auto &arg : args)
        std::cerr << arg << " ";
    std::cerr << std::endl;
}

namespace AI {
void AI::ServerAI::welcomeCommand(std::vector<std::string> &args) {
    (void) args;
    sendDatasToServer("t1\n");
}
}  // namespace AI
