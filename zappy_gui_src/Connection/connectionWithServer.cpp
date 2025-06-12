#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>
#include <netinet/in.h>
#include <string.h>

#include <string>
#include <stdexcept>
#include <iostream>

#include "include/GuiConnection.hpp"
#include "Connection/ServerGUI.hpp"
#include "DataManager/DataManager.hpp"
#include "Exceptions/DataManagerExceptions.hpp"
#include "include/logs.h"

int loopClient(int sockfd) {
    try {
        GUI::ServerGUI::i().server_fd = sockfd;
        GUI::ServerGUI::i().fd = {sockfd, .events = POLLIN | POLLOUT, 0};

        GUI::ServerGUI::i().startServer();
    } catch (const std::exception &e) {
        LOG_WARNING("Server closed");
        close(sockfd);
    }
    return 0;
}

int client_connection(int &sockfd) {
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        throw GUI::InvalidDataException("Socket creation failed");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, GUI::DataManager::i().getIp().c_str(),
        &serv_addr.sin_addr) <= 0)
        throw GUI::InvalidDataException("Invalid IP address");
    serv_addr.sin_port = htons(GUI::DataManager::i().getPort());
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        throw GUI::InvalidDataException("Connection failed");
    return 0;
}
