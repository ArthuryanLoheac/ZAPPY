#include <poll.h>
#include <unistd.h>
#include "DataManager.hpp"
#include "GuiConnection.hpp"
#include "ServerGuiConnection.hpp"
#include <stdexcept>
#include <string.h>
#include <iostream>

static void checkCommand()
{
    while (GUI::ServerGuiConnection::i().buffer.find("\n") != std::string::npos) {
        size_t pos = GUI::ServerGuiConnection::i().buffer.find("\n");
        std::string command = GUI::ServerGuiConnection::i().buffer.substr(0, pos);
        GUI::ServerGuiConnection::i().buffer.erase(0, pos + 1);

        if (command.empty())
            continue;

        std::vector<std::string> args = GUI::ServerGuiConnection::i().parseCommands(command);
        if (args.empty())
            continue;

        auto it = GUI::ServerGuiConnection::i().commands.find(args[0]);
        if (it != GUI::ServerGuiConnection::i().commands.end()) {
            (GUI::ServerGuiConnection::i().*(it->second))(args);
        } else {
            std::cerr << "Unknown command: " << args[0] << std::endl;
        }
    }
}

static void readDatasFromServer(int sockfd)
{
    char buffer[1024];
    ssize_t bytes_read = 0;

    bytes_read = read(sockfd, buffer, sizeof(buffer) - 1);
    if (bytes_read <= 0)
        throw std::runtime_error("Error reading from server");
    buffer[bytes_read] = '\0';
    GUI::ServerGuiConnection::i().buffer.append(buffer);
    checkCommand();
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