#pragma once
#include <poll.h>

#include <string>
#include <map>
#include <vector>

namespace GUI {
class ServerGUI {
 public:
    ServerGUI();
    static ServerGUI &i() {
        static ServerGUI i;
        return i;
    }
    void handleCommand();
    int server_fd;
    int nb_fds;
    struct pollfd fd;

    std::string buffer;

 private:
    std::vector<std::string> parseCommands(std::string &command);

    std::map<std::string, void(ServerGUI::*)
        (std::vector<std::string> &)> commands = {
        {"WELCOME", &ServerGUI::welcomeCommand},
    };

    void welcomeCommand(std::vector<std::string> &args);
    void sendDatasToServer(const std::string &message);
};

}  // namespace GUI
