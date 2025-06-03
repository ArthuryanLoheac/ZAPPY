#include <string.h>
#include <thread>
#include <cstdio>
#include <iostream>

#include "DataManager/DataManager.hpp"
#include "DataManager/GameDataManager.hpp"
#include "include/GuiConnection.hpp"
#include "Exceptions/DataManagerExceptions.hpp"

int checkArgs(int ac, char **av) {
    int i = 1;

    while (i < ac) {
        if (std::string(av[i]) == "-d") {
            GUI::DataManager::i().setDebug(GUI::DataManager::ALL_DEBUG);
            i++;
        } else if (std::string(av[i]) == "-e") {
            GUI::DataManager::i().setDebug(GUI::DataManager::ERRORS);
            i++;
        } else if (std::string(av[i]) == "-p") {
            GUI::DataManager::i().setPort(atoi(av[i + 1]));
            if (GUI::DataManager::i().getPort() < 0 ||
                GUI::DataManager::i().getPort() > 65535)
                return 84;
            i+= 2;
        } else if (std::string(av[i]) == "-h") {
            GUI::DataManager::i().setIp(av[i + 1]);
            i+= 2;
        } else {
            return 84;
        }
    }
    return 0;
}

int returnHelp() {
    std::cout << "Usage: ./zappy_gui -h <ip> -p <port> [-d]\n"
              << "Options:\n"
              << "  -h <ip>   : Set the server IP address\n"
              << "  -p <port> : Set the server port (0-65535)\n"
              << "  -d        : Enable debug mode\n";
    return 84;
}

int main(int ac, char **av) {
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
}
