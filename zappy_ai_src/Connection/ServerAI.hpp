#pragma once
#include <poll.h>

#include <string>
#include <queue>
#include <vector>
#include <map>

namespace AI {
class ServerAI {
 public:
    int server_fd;
    int nb_fds;
    struct pollfd fd;
    std::string buffer;
    bool waitingPos = false;
    bool waitingId = false;

    ServerAI();
    static ServerAI &i() {
        static ServerAI i;
        return i;
    }
    void startServer();

    // Message handling methods
    std::string getNextMessage();
    void queueMessage(const std::string &message);
    void sendQueuedMessages();
    void sendDatasToServer(const std::string &message);

    std::string lastCommand;

 private:
    std::queue<std::string> sendQueue;  // Queue for outgoing messages

    void handleCommand();
    void readDatasFromServer();
    std::vector<std::string> parseCommands(std::string &command);
    
    // Only keep welcome handshake methods
    void welcomeCommand(std::vector<std::string> &args);
    void returnWelcomeCommand(std::vector<std::string> &args);

    // Minimal command map for welcome only
    std::map<std::string, void(ServerAI::*)(std::vector<std::string> &)> commands = {
        {"WELCOME", &ServerAI::welcomeCommand}
    };
};

}  // namespace AI
