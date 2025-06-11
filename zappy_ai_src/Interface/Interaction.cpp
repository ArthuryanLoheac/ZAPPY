#include <string>
#include <vector>

#include "Interface/Interface.hpp"
#include "Exceptions/Commands.hpp"
#include "Data/Data.hpp"

namespace AI {

void Interface::commandEJECT(std::vector<std::string> &args,
    std::vector<std::string> &command) {
    (void)command;
    if (args.size() != 1) {
        throw AI::CommandArgumentsException("EJECT",
            "Expected one argument, got " +
            std::to_string(args.size() - 1));
    }
}

}  // namespace AI
