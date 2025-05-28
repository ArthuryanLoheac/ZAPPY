#include <string.h>
#include "ClientGui.hpp"
#include "DataManager.hpp"
#include "include/GuiConnection.hpp"
#include "DataManagerExceptions.hpp"
#include <thread>

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
    GUI::DataManager dataManager;
    GUI::ClientGui client(sockfd);

    try {
        if (!(ac == 5 || ac == 6))
            return 84;
        if (checkArgs(ac, av) == 84)
            return 84;
        if (client_connection(sockfd) == 84)
            throw ConnectionException("Failed to connect to server");
    } catch (std::exception &e) {
        fprintf(stderr, "Error: %s\n", e.what());
        return 84;
    }
    std::thread t1(graphic, sockfd);
    std::thread t2(loopClient, sockfd);
    t1.join();
    t2.join();
    return 0;
}
