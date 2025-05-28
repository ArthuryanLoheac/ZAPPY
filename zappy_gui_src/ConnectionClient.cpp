#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <netinet/in.h>
#include <string.h>

#include <string>
#include <iostream>

#include "DataManager.hpp"
#include "Exceptions/DataManagerExceptions.hpp"

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
