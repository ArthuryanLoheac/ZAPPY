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

static void readDatasFromServer(int sockfd) {
    char buffer[1024];
    ssize_t bytes_read = 0;

    bytes_read = read(sockfd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0)
        throw std::runtime_error("Error reading from server");
    buffer[bytes_read] = '\0';
    GUI::ServerGUI::i().buffer.append(buffer);
    GUI::ServerGUI::i().handleCommand();
}

void start_server() {
    int ready = 0;

    while (1) {
        ready = poll(
            &GUI::ServerGUI::i().fd, 1, -1);
        if (ready == -1)
            throw std::runtime_error("Poll error occurred");
        if (GUI::ServerGUI::i().fd.revents & POLLIN) {
            // Read data from the server
            readDatasFromServer(GUI::ServerGUI::i().server_fd);
        }
    }
}

int loopClient(int sockfd) {
    GUI::ServerGUI::i().server_fd = sockfd;
    GUI::ServerGUI::i().fd = {sockfd, .events = POLLIN | POLLOUT, 0};
    start_server();
    return 0;
}

int client_connection(int &sockfd) {
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        throw InvalidDataException("Socket creation failed");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, GUI::DataManager::i().getIp().c_str(),
        &serv_addr.sin_addr) <= 0)
        throw InvalidDataException("Invalid IP address");
    serv_addr.sin_port = htons(GUI::DataManager::i().getPort());
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        throw InvalidDataException("Connection failed");
    return 0;
}