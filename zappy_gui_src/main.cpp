#include <string.h>
#include <thread>
#include <cstdio>

#include "DataManager/DataManager.hpp"
#include "DataManager/GameDataManager.hpp"
#include "include/GuiConnection.hpp"
#include "Exceptions/DataManagerExceptions.hpp"

int checkArgs(int ac, char **av) {
    int i = 1;

    while (i < ac) {
        if (strcmp(av[i], "-d") == 0) {
            GUI::DataManager::i().setDebug(true);
            i++;
        } else if (strcmp(av[i], "-p") == 0) {
            GUI::DataManager::i().setPort(atoi(av[i + 1]));
            if (GUI::DataManager::i().getPort() < 0 ||
                GUI::DataManager::i().getPort() > 65535)
                return 84;
            i+= 2;
        } else if (strcmp(av[i], "-h") == 0) {
            GUI::DataManager::i().setIp(av[i + 1]);
            i+= 2;
        } else {
            return 84;
        }
    }
    return 0;
}

int main(int ac, char **av) {
    int sockfd;
    GUI::DataManager::i();
    GUI::GameDataManager::i();

    try {
        if (!(ac == 5 || ac == 6))
            return 84;
        if (checkArgs(ac, av) == 84)
            return 84;
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
        fprintf(stderr, "Error in graphic(): %s\n", e.what());
        GUI::DataManager::i().setRunning(false);
        communicationThread.join();
        return 84;
    }
    return 0;
}
