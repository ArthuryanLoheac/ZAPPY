#include <iostream>
#include <string>
#include <vector>
#include <csignal>
#include <chrono>
#include <thread>
#include <memory>

#include "Exceptions/Factory.hpp"
#include "ForkWrapper/Fork.hpp"
#include "Socket/Socket.hpp"
#include "Interface/Interface.hpp"
#include "Data/Data.hpp"

bool sigintReceived = false;
bool usr1Received = false;

void setupSignalHandlers() {
    std::signal(SIGINT, [](int) { sigintReceived = true; });
    std::signal(SIGUSR1, [](int) { usr1Received = true; });
}

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
        exit(0);
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
    AI::Interface &interface = AI::Interface::i();
    try {
        interface.start(port, ip, name);
    } catch (const AI::FactoryException &e) {
        std::cerr << "Error starting interface: " << e.what() << std::endl;
        return 84;
    }

    while (AI::Data::i().isDead == false) {
        try {
            interface.run();
        } catch (const AI::ChildProcessException &e) {
            std::cerr << "Child process error: " << e.what() << std::endl;
            return 84;
        } catch (const AI::FactoryException &e) {
            std::cerr << "Factory error: " << e.what() << std::endl;
            return 84;
        }
        if (AI::Data::i().isRunning) {
            try {
                interface.sendCommand(FORWARD);
                interface.sendCommand(INVENTORY);
                interface.sendCommand("Set food\n");
            } catch (const std::exception &e) {
                std::cerr << "Error sending command: " << e.what() << std::endl;
            }
        }
    }
    return 0;
}

int mainLoop(int port, const std::string &ip,
    const std::string &name) {
    std::vector<std::unique_ptr<Fork>> childs;

    setupSignalHandlers();

    childs.push_back(std::make_unique<Fork>(initChildProcess, port, ip, name));

    while (!sigintReceived) {
        for (auto it = childs.begin(); it != childs.end();) {
            if ((*it)->waitNoHang()) {
                std::cout << "Child process PID " << (*it)->getPid()
                    << " has exited with status "
                    << (*it)->getExitStatus() << std::endl;
                it = childs.erase(it);
            } else {
                ++it;
            }
        }

        if (usr1Received) {
            childs.push_back(
                std::make_unique<Fork>(initChildProcess, port, ip, name));
            usr1Received = false;
        }

        // if (countForForks >= 2) {
        //     childs.push_back(
        //         std::make_unique<Fork>(initChildProcess, port, ip, name));
        //     countForForks = 0;
        // }

        if (childs.empty()) {
            std::cout << "No child processes running, exiting." << std::endl;
            return 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    for (auto &child : childs) {
        child->terminateChild();
        child->wait();
    }
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

    return mainLoop(port, ip, name);
}
