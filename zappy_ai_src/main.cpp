/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/

#include <string.h>
#include <thread>
#include <cstdio>
#include <iostream>
#include <string>

#include "DataManager/DataManager.hpp"
#include "include/AIConnection.hpp"
#include "Exceptions/DataManagerExceptions.hpp"

int checkArgs(int ac, char **av) {
    int i = 1;

    while (i < ac) {
        if (std::string(av[i]) == "-d") {
            AI::DataManager::i().setDebug(AI::DataManager::ALL_DEBUG);
            i++;
        } else if (std::string(av[i]) == "-e") {
            AI::DataManager::i().setDebug(AI::DataManager::ERRORS);
            i++;
        }  else if (std::string(av[i]) == "-n" && i + 1 < ac) {
            AI::DataManager::i().setTeam(av[i + 1]);
            i+= 2;
        } else if (std::string(av[i]) == "-p" && i + 1 < ac) {
            AI::DataManager::i().setPort(atoi(av[i + 1]));
            if (AI::DataManager::i().getPort() < 0 ||
                AI::DataManager::i().getPort() > 65535)
                return 84;
            i+= 2;
        } else if (std::string(av[i]) == "-h" && i + 1 < ac) {
            AI::DataManager::i().setIp(av[i + 1]);
            i+= 2;
        } else {
            return 84;
        }
    }
    return 0;
}

int returnHelp() {
    std::cout << "Usage: ./zappy_ai -h <ip> -p <port> -n <teamName> [-d]\n"
              << "Options:\n"
              << "  -h <ip>   : Set the server IP address\n"
              << "  -p <port> : Set the server port (0-65535)\n"
              << "  -n <teamName> : Set the team name\n"
              << "  -d        : Enable debug mode\n";
    return 84;
}

int main(int ac, char **av) {
    int sockfd;
    AI::DataManager::i();

    try {
        if (!(ac == 7 || ac == 8))
            return returnHelp();
        if (checkArgs(ac, av) == 84)
            return returnHelp();
        if (client_connection(sockfd) == 84)
            throw AI::ConnectionException("Failed to connect to server");
    } catch (std::exception &e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return 84;
    }
    loopClient(sockfd);
    AI::DataManager::i().setRunning(false);

    return 0;
}