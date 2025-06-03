#pragma once
#include <poll.h>

#include <string>
#include <map>
#include <vector>
#include <chrono>

namespace GUI {
class ServerGUI {
 public:
    int server_fd;
    int nb_fds;
    struct pollfd fd;
    std::string buffer;
    int updateMapTime = 3;

    ServerGUI();
    static ServerGUI &i() {
        static ServerGUI i;
        return i;
    }
    void startServer();

 private:
    void clockUpdate(std::chrono::_V2::system_clock::time_point &time,
        std::chrono::_V2::system_clock::time_point &timeNext);
    void handleCommand();
    void readDatasFromServer();
    std::vector<std::string> parseCommands(std::string &command);
    void sendDatasToServer(const std::string &message);

    void welcomeCommand(std::vector<std::string> &args);
    void mszCommand(std::vector<std::string> &args);
    void enwCommand(std::vector<std::string> &args);
    void tnaCommand(std::vector<std::string> &args);
    void bctCommand(std::vector<std::string> &args);
    void sgtCommand(std::vector<std::string> &args);
    void eboCommand(std::vector<std::string> &args);

    std::map<std::string,
     void(ServerGUI::*)(std::vector<std::string> &)> commands = {
        {"WELCOME", &ServerGUI::welcomeCommand},
        {"MSZ", &ServerGUI::mszCommand},
        {"ENW", &ServerGUI::enwCommand},
        {"TNA", &ServerGUI::tnaCommand},
        {"BCT", &ServerGUI::bctCommand},
        {"SGT", &ServerGUI::sgtCommand},
        {"EBO", &ServerGUI::eboCommand},
    };
};

}  // namespace GUI
