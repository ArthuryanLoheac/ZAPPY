#pragma once
#include <poll.h>

#include <string>
#include <map>
#include <vector>
#include <chrono>

namespace AI {
class ServerAI {
 public:
    int server_fd;
    int nb_fds;
    struct pollfd fd;
    std::string buffer;
    int updateMapTime = 30;

    ServerAI();
    static ServerAI &i() {
        static ServerAI i;
        return i;
    }
    void startServer();

 private:
    void handleCommand();
    void readDatasFromServer();
    std::vector<std::string> parseCommands(std::string &command);
    void execCommand(std::map<std::string, void(AI::ServerAI::*)
    (std::vector<std::string> &)>::iterator it, std::vector<std::string> &args);
    void sendDatasToServer(const std::string &message);

    void welcomeCommand(std::vector<std::string> &args);

    std::map<std::string,
     void(ServerAI::*)(std::vector<std::string> &)> commands = {
        {"WELCOME", &ServerAI::welcomeCommand}
    };
};

}  // namespace AI
