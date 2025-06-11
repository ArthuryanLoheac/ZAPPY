#pragma once

#include <queue>
#include <string>
#include <map>
#include <vector>

#include "Socket/Socket.hpp"

namespace AI {

#define FORWARD "Forward\n"
#define LEFT "Left\n"
#define RIGHT "Right\n"
#define LOOK "Look\n"
#define INVENTORY "Inventory\n"
#define CONNECT_NBR "Connect_nbr\n"
#define FORK "Fork\n"
#define EJECT "Eject\n"
#define INCANTATION "Incantation\n"

class Interface {
 public:
    Interface();
    ~Interface();

    static Interface &i() {
        static Interface instance;
        return instance;
    }

    void start(int port, const std::string &ip, const std::string &name);
    void run();
    void stop();
    void sendCommand(const std::string &command);

 private:
    void handleQueues();

    Network::Socket socket;

    int port;
    std::string ip;
    std::string name;

    std::queue<std::vector<std::string>> inputQueue;
    std::queue<std::vector<std::string>> outputQueue;
    std::map<std::string, void(Interface::*)(std::vector<std::string> &,
        std::vector<std::string> &)> commands;

    // Receiving commands
    void factoryCommands();

    void commandWELCOME(std::vector<std::string> &args,
        std::vector<std::string> &command);
    void commandFORWARD(std::vector<std::string> &args,
        std::vector<std::string> &command);
    void commandLEFT(std::vector<std::string> &args,
        std::vector<std::string> &command);
    void commandRIGHT(std::vector<std::string> &args,
        std::vector<std::string> &command);
    void commandLOOK(std::vector<std::string> &args,
        std::vector<std::string> &command);
    void commandINVENTORY(std::vector<std::string> &args,
        std::vector<std::string> &command);
    void commandTAKE(std::vector<std::string> &args,
        std::vector<std::string> &command);
    void commandSET(std::vector<std::string> &args,
        std::vector<std::string> &command);

    // Sending commands
    std::queue<std::string> commandBuffer;
};

}  // namespace AI
