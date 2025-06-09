#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <poll.h>
#include <netinet/in.h>
#include <string.h>

#include <string>
#include <stdexcept>
#include <iostream>

#include "include/AIConnection.hpp"
#include "Connection/ServerAI.hpp"
#include "DataManager/DataManager.hpp"
#include "Exceptions/DataManagerExceptions.hpp"

int loopClient(int sockfd) {
    try {
        AI::ServerAI::i().server_fd = sockfd;
        AI::ServerAI::i().fd = {sockfd, POLLIN | POLLOUT, 0};

        AI::ServerAI::i().startServer();
    } catch (const std::exception &e) {
        std::cerr << "Server Closed: " << e.what() << std::endl;
        close(sockfd);
    }
    return 0;
}

int client_connection(int &sockfd) {
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        throw AI::InvalidDataException("Socket creation failed");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, AI::DataManager::i().getIp().c_str(),
        &serv_addr.sin_addr) <= 0)
        throw AI::InvalidDataException("Invalid IP address");
    serv_addr.sin_port = htons(AI::DataManager::i().getPort());
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        throw AI::InvalidDataException("Connection failed");
    return 0;
}
