#pragma once
#include <string>
#include <map>
#include <vector>

namespace GUI
{
class ServerGuiConnection {
 public:
    ServerGuiConnection();
    static ServerGuiConnection &i() {
        static ServerGuiConnection i;
        return i;
    }
    void handleCommand(std::string &command);
    int server_fd;
    int nb_fds;
    struct pollfd fd;
 private:


    std::vector<std::string> parseCommands(std::string &command);

    std::map<std::string, void (ServerGuiConnection::*)(std::vector<std::string> &)> commands = {
        {"WELCOME", &ServerGuiConnection::welcomeCommand},
    };

    void welcomeCommand(std::vector<std::string> &args);
};

} // namespace GUI