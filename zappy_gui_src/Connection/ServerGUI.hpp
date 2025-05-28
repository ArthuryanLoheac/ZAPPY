#pragma once
#include <poll.h>

#include <string>
#include <map>
#include <vector>

namespace GUI {
class ServerGUI {
 public:
    int server_fd;
    int nb_fds;
    struct pollfd fd;
    std::string buffer;

    ServerGUI();
    static ServerGUI &i() {
        static ServerGUI i;
        return i;
    }
    void startServer();

 private:
    void handleCommand();
    void readDatasFromServer();
    std::vector<std::string> parseCommands(std::string &command);
    void sendDatasToServer(const std::string &message);

    void welcomeCommand(std::vector<std::string> &args);
    void mszCommand(std::vector<std::string> &args);

    std::map<std::string,
     void(ServerGUI::*)(std::vector<std::string> &)> commands = {
        {"WELCOME", &ServerGUI::welcomeCommand},
        {"MSZ", &ServerGUI::mszCommand},
    };
};

}  // namespace GUI
