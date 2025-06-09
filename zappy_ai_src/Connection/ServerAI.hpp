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
    int updateMapTime = 3;
    bool waitingPos = false;
    bool waitingId = false;

    ServerAI();
    static ServerAI &i() {
        static ServerAI i;
        return i;
    }
    void startServer();

    std::string lastCommand;

 private:
    void handleCommand();
    void readDatasFromServer();
    std::vector<std::string> parseCommands(std::string &command);
    void execCommand(std::map<std::string, void(AI::ServerAI::*)
    (std::vector<std::string> &)>::iterator it, std::vector<std::string> &args);
    void execReturnCommand(std::map<std::string, void(AI::ServerAI::*)
    (std::vector<std::string> &)>::iterator it, std::vector<std::string> &args);
    void sendDatasToServer(const std::string &message);
    void handleReturnValue(std::vector<std::string> &args);

    void welcomeCommand(std::vector<std::string> &args);
    void returnWelcomeCommand(std::vector<std::string> &args);
    void takeFoodCommand(std::vector<std::string> &args);
    void forwardCommand(std::vector<std::string> &args);

    std::map<std::string,
     void(ServerAI::*)(std::vector<std::string> &)> commands = {
        {"WELCOME", &ServerAI::welcomeCommand}
    };
    std::map<std::string,
     void(ServerAI::*)(std::vector<std::string> &)> LastCommands = {
        {"Forward\n", &ServerAI::forwardCommand},
        {"Take food\n", &ServerAI::takeFoodCommand},
    };
};

}  // namespace AI
