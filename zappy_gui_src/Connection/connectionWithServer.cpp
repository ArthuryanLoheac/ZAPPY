#include <poll.h>
#include <unistd.h>
#include "DataManager.hpp"
#include "GuiConnection.hpp"
#include "ServerGuiConnection.hpp"
#include <stdexcept>
#include <string.h>
#include <iostream>

static void readDatasFromServer(int sockfd)
{
    char buffer[1024];
    ssize_t bytes_read = 0;

    bytes_read = read(sockfd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0)
        throw std::runtime_error("Error reading from server");
    buffer[bytes_read] = '\0';
    GUI::ServerGuiConnection::i().buffer.append(buffer);
    GUI::ServerGuiConnection::i().handleCommand();
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