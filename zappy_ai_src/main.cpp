#include <iostream>
#include <string>

#include "Exceptions/Factory.hpp"
#include "ForkWrapper/Fork.hpp"
#include "lib/Socket/Socket.hpp"

void printHelp() {
    std::cout << "USAGE: ./zappy_ai -p port -n name -h machine" << std::endl <<
        "  -p port:    port number of the zappy server" << std::endl <<
        "  -h machine: machine name or IP address of the zappy server"
        << std::endl;
}

void parseArgs(int argc, char **argv, std::string &ip, int &port) {
    if (argc < 2 || argc > 3) {
        throw AI::ArgumentsException("Invalid number of arguments.");
    }

    if (argc == 2 && std::string(argv[1]) == "--help") {
        printHelp();
        return;
    }

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-p") {
            if (i + 1 < argc) {
                port = std::stoi(argv[++i]);
            } else {
                throw AI::ArgumentsException("Port number is missing.");
            }
        } else if (std::string(argv[i]) == "-h") {
            if (i + 1 < argc) {
                ip = argv[++i];
            } else {
                throw AI::ArgumentsException("IP address is missing.");
            }
        } else {
            throw AI::ArgumentsException("Unknown argument: " +
                std::string(argv[i]));
        }
    }

    if (port <= 0 || port > 65535) {
        throw
            AI::ArgumentsException("Port number must be between 1 and 65535.");
    }
}

void initChildProcess(int port, const std::string &ip) {
    Network::Socket socket(port, ip);
}

int main(int argc, char **argv) {
    std::string ip = "localhost";
    int port = 0;

    try {
        parseArgs(argc, argv, ip, port);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    Fork(initChildProcess, port, ip);
    return 0;
}
