#pragma once

#include <poll.h>

#include <string>
#include <map>
#include <vector>

namespace Network {

class Socket {
 public:
    int port;
    std::string ip;
    int server_fd;
    int nb_fds;
    struct pollfd fd;
    bool running;
    std::string buffer;

    Socket();
    explicit Socket(int port, const std::string &ip = "localhost");
    ~Socket();

    static Socket &i() {
        static Socket i;
        return i;
    }

    void startSocket();
    void startSocket(int port, const std::string &ip = "localhost");
    void run();
    void stopSocket();

    void sendDatasToServer(const std::string &message) const;

    std::vector<std::vector<std::string>> getListOutputs();

 private:
    void handleCommand();
    void readDatasFromServer();

    static std::vector<std::string> parseCommands(std::string &command);
    std::vector<std::vector<std::string>> listOuputs;
};

}  // namespace Network
