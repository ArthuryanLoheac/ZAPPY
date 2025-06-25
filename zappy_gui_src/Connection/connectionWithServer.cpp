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

/**
 * @brief Handles the client-server communication loop.
 *
 * @param sockfd The socket file descriptor for the server connection.
 * @return int Always returns 0.
 */
int loopClient(int sockfd) {
    try {
        GUI::ServerGUI::i().initNetwork(sockfd);

        GUI::ServerGUI::i().startServer();
    } catch (const std::exception &e) {
        GUI::ServerGUI::i().setConnectedToServer(false);
        LOG_WARNING("Server closed");
        close(sockfd);
        bool reconnected = false;
        while (!reconnected) {
            try {
                reconnected = (client_connection(sockfd) != 84);
            } catch (const GUI::InvalidDataException &e) {
                sleep(1);
            }
        }
        loopClient(sockfd);
    }
    return 0;
}

/**
 * @brief Establishes a connection to the server.
 *
 * @param sockfd Reference to the socket file descriptor to be initialized.
 * @return int Returns 0 on success.
 * @throws GUI::InvalidDataException If socket creation, IP address conversion, or connection fails.
 */
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
