#include <poll.h>
#include <unistd.h>
#include "DataManager.hpp"
#include "GuiConnection.hpp"
#include "ServerGuiConnection.hpp"

int loopClient(int sockfd) {
    printf("Starting client loop...\n");
    GUI::ServerGuiConnection::i().setFdServer(sockfd);
    return 0;
}