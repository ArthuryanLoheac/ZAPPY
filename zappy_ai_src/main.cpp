#include <netdb.h>
#include <arpa/inet.h>

#include <string>
#include <vector>
#include <csignal>
#include <cstring>
#include <chrono>
#include <thread>
#include <memory>
#include <iostream>
#include <stdexcept>

#include "Exceptions/Factory.hpp"
#include "ForkWrapper/Fork.hpp"
#include "Interface/Interface.hpp"
#include "Data/Data.hpp"
#include "include/logs.h"
#include "Logic/Core.hpp"
#include "modules/FoodGatheringModule.hpp"
#include "modules/CommunicationModule.hpp"
#include "modules/ElevationModule.hpp"


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
        "  -h machine: machine name or IP address of the zappy server" <<
        "  -v, -vv, -vvv : Set verbose level (WARNING, INFO, DEBUG)"
        << std::endl;
}

std::string getHostnameIP(const std::string& hostname) {
    struct addrinfo hints{}, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (const int status = getaddrinfo(hostname.c_str(), nullptr, &hints, &res);
        status != 0) {
        throw std::runtime_error("Error while getting IP: " +
            std::string(gai_strerror(status)));
    }

    const auto *ipv4 = reinterpret_cast<struct sockaddr_in *>(res->ai_addr);
    char ipBuffer[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(ipv4->sin_addr), ipBuffer, sizeof(ipBuffer));
    std::string ipStr = ipBuffer;
    freeaddrinfo(res);
    return ipStr;
}

void parseArgs(const int argc, char **argv, std::string &ip, int &port,
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
        } else if (std::string(argv[i]) == "-v") {
            set_log_level(WARNING);
        } else if (std::string(argv[i]) == "-vv") {
            set_log_level(INFO);
        } else if (std::string(argv[i]) == "-vvv") {
            set_log_level(DEBUG);
        } else {
            throw AI::ArgumentsException("Unknown argument: " +
                std::string(argv[i]));
        }
    }

    if (port <= 0 || port > 65535) {
        throw
            AI::ArgumentsException("Port number must be between 1 and 65535.");
    }

    ip = getHostnameIP(ip);

    if (name.empty()) {
        throw AI::ArgumentsException("Team name cannot be empty.");
    }
}

int initChildProcess(const int port, const std::string &ip,
    const std::string &name) {
    AI::Interface &interface = AI::Interface::i();
    try {
        interface.start(port, ip, name);
    } catch (const AI::FactoryException &e) {
        return 42;
    }

    Logic& logic = Logic::getInstance();
    logic.addModule(std::make_unique<FoodGatheringModule>());
    logic.addModule(std::make_unique<CommunicationModule>());
    logic.addModule(std::make_unique<ElevationModule>());

    while (AI::Data::i().isDead == false) {
        try {
            interface.run();
        } catch (const AI::ChildProcessException &e) {
            LOG_ERROR("Child process error in PID %d: %s", getpid(), e.what());
            return 84;
        } catch (const AI::FactoryException &e) {
            LOG_ERROR("Factory error in PID %d: %s", getpid(), e.what());
            return 84;
        }
        if (AI::Data::i().isRunning) {
            try {
                if (!interface.isWaitingForResponse()) {
                    logic.executeHighestPriorityModule();
                }
            } catch (const std::exception &e) {
                LOG_WARNING("Error sending command in PID %d: %s",
                    getpid(), e.what());
            }
        }
    }
    return 0;
}

int mainLoop(int port, const std::string &ip,
    const std::string &name) {
    std::vector<std::unique_ptr<Fork>> childs;

    setupSignalHandlers();
    int lastErrorCode = 0;

    childs.push_back(std::make_unique<Fork>(initChildProcess, port, ip, name));

    while (!sigintReceived) {
        for (auto it = childs.begin(); it != childs.end();) {
            if ((*it)->waitNoHang()) {
                LOG_INFO("Child process PID %d has exited with status %d.",
                    (*it)->getPid(), (*it)->getExitStatus());
                lastErrorCode = (*it)->getExitStatus();
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

        if (childs.empty()) {
            if (lastErrorCode == 42) {
                LOG_FATAL("The AI can't connect to the server, exiting.");
                return 84;
            }
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
    std::string name;
    int port = 0;

    try {
        parseArgs(argc, argv, ip, port, name);
    } catch (const std::exception &e) {
        LOG_FATAL("Argument parsing error: %s", e.what());
        return 84;
    }

    return mainLoop(port, ip, name);
}
