#include <string.h>
#include <thread>
#include <cstdio>
#include <iostream>
#include <string>

#include "DataManager/DataManager.hpp"
#include "DataManager/GameDataManager.hpp"
#include "include/GuiConnection.hpp"
#include "Exceptions/DataManagerExceptions.hpp"
#include "include/logs.h"

/**
 * @brief Checks the command-line arguments and sets the appropriate configurations.
 * @param ac Argument count.
 * @param av Argument vector.
 * @return int Returns 0 on success, 84 on error.
 */
int checkArgs(int ac, char **av) {
    int i = 1;

    while (i < ac) {
        if (std::string(av[i]) == "-p" && i + 1 < ac) {
            GUI::DataManager::i().setPort(atoi(av[i + 1]));
            if (GUI::DataManager::i().getPort() < 0 ||
                GUI::DataManager::i().getPort() > 65535)
                return 84;
            i+= 2;
        } else if (std::string(av[i]) == "-h" && i + 1 < ac) {
            GUI::DataManager::i().setIp(av[i + 1]);
            i+= 2;
        } else if (std::string(av[i]) == "-v") {
            set_log_level(WARNING);
            i++;
        } else if (std::string(av[i]) == "-vv") {
            set_log_level(INFO);
            i++;
        } else if (std::string(av[i]) == "-vvv") {
            set_log_level(DEBUG);
            i++;
        } else {
            return 84;
        }
    }
    return 0;
}

/**
 * @brief Displays the help message for the program usage.
 * @return int Always returns 84.
 */
int returnHelp() {
    std::cout << "Usage: ./zappy_gui -h <ip> -p <port> [-d]\n"
              << "Options:\n"
              << "  -h <ip>       : Set the server IP address\n"
              << "  -p <port>     : Set the server port (0-65535)\n"
              << "  -v, -vv, -vvv : Set verbose level (WARNING, INFO, DEBUG\n";
    return 84;
}

/**
 * @brief Entry point of the program. Initializes the application, processes arguments,
 *        establishes a connection, and starts the graphical interface.
 * @param ac Argument count.
 * @param av Argument vector.
 * @return int Returns 0 on success, 84 on failure.
 */
int main(int ac, char **av) {
    try {
        int sockfd;
        GUI::DataManager::i();
        GUI::GameDataManager::i();

        try {
            if (!(ac == 5 || ac == 6))
                return returnHelp();
            if (checkArgs(ac, av) == 84)
                return returnHelp();
            if (client_connection(sockfd) == 84)
                throw GUI::ConnectionException("Failed to connect to server");
        } catch (std::exception &e) {
            fprintf(stderr, "Error: %s\n", e.what());
            return 84;
        }
        std::thread communicationThread(loopClient, sockfd);
        try {
            graphic();
        } catch (std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
            GUI::DataManager::i().setRunning(false);
            communicationThread.join();
            return 84;
        }
        GUI::DataManager::i().setRunning(false);
        communicationThread.join();
        return 0;
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 84;
    }
}
