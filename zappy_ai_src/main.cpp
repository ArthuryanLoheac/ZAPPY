#include <iostream>
#include <string>

#include "Exceptions/Factory.hpp"
#include "ForkWrapper/Fork.hpp"
#include "Socket/Socket.hpp"
#include "Interface/Interface.hpp"

void printHelp() {
    std::cout << "USAGE: ./zappy_ai -p port -n name -h machine" << std::endl <<
        "  -n name:    name of the team of the AI" << std::endl <<
        "  -p port:    port number of the zappy server" << std::endl <<
        "  -h machine: machine name or IP address of the zappy server"
        << std::endl;
}

void parseArgs(int argc, char **argv, std::string &ip, int &port,
    std::string &name) {
    if (argc < 2) {
        throw AI::ArgumentsException("Invalid number of arguments.");
    }

    if (argc == 2 && std::string(argv[1]) == "--help") {
        printHelp();
        return;
    }

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-p" && i + 1 < argc) {
            port = std::stoi(argv[++i]);
        } else if (std::string(argv[i]) == "-n" && i + 1 < argc) {
            name = argv[++i];
        } else if (std::string(argv[i]) == "-h" && i + 1 < argc) {
            ip = argv[++i];
        } else {
            throw AI::ArgumentsException("Unknown argument: " +
                std::string(argv[i]));
        }
    }

    if (port <= 0 || port > 65535) {
        throw
            AI::ArgumentsException("Port number must be between 1 and 65535.");
    }

    if (name.empty()) {
        throw AI::ArgumentsException("Team name cannot be empty.");
    }
}

int initChildProcess(int port, const std::string &ip,
    const std::string &name) {
    Network::Socket socket;
    try {
        socket.startSocket(port, ip);
    } catch (const std::exception &e) {
        std::cerr << "Child PID " << getpid() << " error: " << e.what()
            << std::endl;
        return 84;
    }
    initSocketCommands(socket);
    while (true) {
        try {
            socket.run();
            auto outputs = socket.getListOutputs();
            for (const auto &output : outputs) {
                if (!output.empty()) {
                    std::cout << "Received command: ";
                    for (const auto &arg : output) {
                        std::cout << arg << " ";
                    }
                    std::cout << std::endl;
                    if (output[0] == "WELCOME") {
                        socket.sendDatasToServer(name + "\n");
                    }
                    outputs.clear();
                }
            }
        } catch (const std::exception &e) {
            std::cerr << "Child PID " << getpid() << " error: " << e.what()
                << std::endl;
            return 84;
        }
    }
    (void)name;
    return 0;
}

int main(int argc, char **argv) {
    std::string ip = "localhost";
    std::string name = "";
    int port = 0;

    try {
        parseArgs(argc, argv, ip, port, name);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
    std::cout << "Zappy AI started with team name: " << name
        << ", port: " << port << ", IP: " << ip << std::endl;
    Fork(initChildProcess, port, ip, name);
    return 0;
}
