#include <poll.h>
#include <unistd.h>
#include "DataManager.hpp"
#include "GuiConnection.hpp"
#include "ServerGuiConnection.hpp"
#include <stdexcept>
#include <string.h>
#include <iostream>

static void sendDatasToServer(int sockfd, struct pollfd &fd, const std::string &message) {
    if (fd.revents & POLLOUT) {
        ssize_t bytes_sent = write(sockfd, message.c_str(), message.size());
        if (bytes_sent == -1) {
            throw std::runtime_error("Error sending data to server");
        }
        printf("Sent data: %s\n", message.c_str());
    }
}

static void readDatasFromServer(int sockfd)
{
    char buffer[1024];
    ssize_t bytes_read = 0;

    bytes_read = read(sockfd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0) {
        if (bytes_read == -1)
            throw std::runtime_error("Error reading from server");
        else
            throw std::runtime_error("Server connection closed");
    }
    buffer[bytes_read] = '\0'; // Null-terminate the string
    std::string data(buffer);
    printf("Received data: %s\n", data.c_str());
    if (data.find("WELCOME") != std::string::npos) {
        sendDatasToServer(sockfd, GUI::ServerGuiConnection::i().fd, "GRAPHIC\n");
    }
}

void start_server()
{
    int ready = 0;

    while (1) {
        ready = poll(
            &GUI::ServerGuiConnection::i().fd, 1, -1);
        if (ready == -1)
            throw std::runtime_error("Poll error occurred");
        if (GUI::ServerGuiConnection::i().fd.revents & POLLIN) {
            // Read data from the server
            readDatasFromServer(GUI::ServerGuiConnection::i().server_fd);
        }
    }
}


int loopClient(int sockfd) {
    GUI::ServerGuiConnection::i().server_fd = sockfd;
    GUI::ServerGuiConnection::i().fd = {sockfd, .events = POLLIN | POLLOUT, 0};
    start_server();
    return 0;
}