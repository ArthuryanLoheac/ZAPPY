#include <poll.h>
#include <unistd.h>
#include "DataManager.hpp"
#include "GuiConnection.hpp"

static void checkCommand(std::string &buffer) {
    size_t pos = buffer.find("\n");
    if (pos != std::string::npos) {
        std::string command = buffer.substr(0, pos);
        buffer.erase(0, pos + 1);
        if (GUI::DataManager::i().getDebug()) {
            printf("Received command: (%s)\n", command.c_str());
        }
        for (size_t i = 0; i < command.size(); ++i) {
            if (command[i] >= 'a' && command[i] <= 'z')
                command[i] -= 32;
        }
        handleCommand(command);
    }
}

static void readDatas(int sockfd, struct pollfd &fds, std::string &buffer) {
    int valread;
    char tempBuffer[1024];

    if (poll(&fds, 1, 0) > 0) {
        if (fds.revents & POLLIN) {
            valread = read(sockfd, tempBuffer, 1024);
            if (valread > 0) {
                buffer.append(tempBuffer);
                checkCommand(buffer);
            }
            fds.revents = 0;
        }
    }
}

int loopClient(int sockfd) {
    struct pollfd fds;
    std::string buffer;

    fds.fd = sockfd;
    fds.events = POLLIN;
    while (GUI::DataManager::i().running) {
        readDatas(sockfd, fds, buffer);
    }
    return 0;
}