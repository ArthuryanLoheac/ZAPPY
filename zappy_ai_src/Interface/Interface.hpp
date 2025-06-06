#pragma once

#include <queue>
#include <string>
#include <map>

#include "Socket/Socket.hpp"

namespace AI {

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

 private:
    void handleQueues();

    Network::Socket socket;

    int port;
    std::string ip;
    std::string name;

    std::queue<std::vector<std::string>> inputQueue;
    std::queue<std::vector<std::string>> outputQueue;
    std::map<std::string, void(Interface::*)(std::vector<std::string> &)>
        commands;

    // Commands
    void factoryCommands();

    void commandWELCOME(std::vector<std::string> &args);
};

}  // namespace AI
