#pragma once

#include <poll.h>

#include <string>
#include <map>
#include <vector>

namespace Network {

class Socket {
 public:
    int server_fd;
    int nb_fds;
    struct pollfd fd;
    bool running;
    std::string buffer;

    Socket();
    explicit Socket(const std::string &ip = "localhost", int port);
    ~Socket();

    static Socket &i() {
        static Socket i;
        return i;
    }

    void startSocket(const std::string &ip, int port);
    void run();
    void stopSocket();

    void addCommand(const std::string &command,
        void(*func)(std::vector<std::string> &));

 private:
    void handleCommand();
    void readDatasFromServer();

    static std::vector<std::string> parseCommands(std::string &command);
    void sendDatasToServer(const std::string &message) const;

    std::map<std::string, void(*)(std::vector<std::string> &)> commands;
};

}  // namespace Network
