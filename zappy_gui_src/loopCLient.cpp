#include <poll.h>
#include "DataManager.hpp"

int loopClient(int sockfd) {
    struct pollfd fds;

    fds.fd = sockfd;
    fds.events = POLLIN;
    while (GUI::DataManager::i().running) {
        (void)fds;
    }
    return 0;
}