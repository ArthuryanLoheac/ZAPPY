#include "Socket/Socket.hpp"

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

namespace Network {

Socket::Socket(): port(0), ip(), server_fd(0), nb_fds(0), fd(), running(false) {
}

Socket::Socket(const int port, const std::string &ip) : port(port), ip(ip),
    server_fd(0), nb_fds(0), fd(), running(false) {
}

Socket::~Socket() {
    if (running) {
        stopSocket();
    }
}

void Socket::startSocket(const int port, const std::string &ip) {
    this->port = port;
    this->ip = ip;
    if (running)
        throw std::runtime_error("Socket is already running");
    startSocket();
}

void Socket::startSocket() {
    if (running)
        throw std::runtime_error("Socket is already running");

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        throw std::runtime_error("Socket creation failed");

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0)
        throw std::runtime_error("Invalid IP address");

    if (connect(server_fd, reinterpret_cast<struct sockaddr *>(&serv_addr),
        sizeof(serv_addr)) < 0)
        throw std::runtime_error("Connection failed");

    fd.fd = server_fd;
    fd.events = POLLIN | POLLOUT;
    running = true;
}

void Socket::run() {
    if (poll(&fd, 1, -1) == -1)
        throw std::runtime_error("Poll error occurred");
    if (fd.revents & POLLIN)
        readDatasFromServer();
}

void Socket::stopSocket() {
    if (running) {
        close(server_fd);
        running = false;
        server_fd = 0;
        buffer.clear();
    }
}

void Socket::handleCommand() {
    while (buffer.find('\n') != std::string::npos) {
        const size_t pos = buffer.find('\n');
        std::string command = buffer.substr(0, pos);
        buffer.erase(0, pos + 1);

        if (command.empty()) continue;

        std::vector<std::string> args =
            parseCommands(command);
        if (args.empty()) continue;

        for (size_t i = 0; i < args[0].length(); ++i)
            args[0][i] = toupper(args[0][i]);

        listOuputs.push_back(args);
    }
}

void Socket::readDatasFromServer() {
    char bufferTemp[1024];
    ssize_t bytes_read = 0;

    bytes_read = read(server_fd,
        bufferTemp, sizeof(bufferTemp) - 1);
    if (bytes_read <= 0)
        throw std::runtime_error("Error reading from server");
    bufferTemp[bytes_read] = '\0';
    buffer.append(bufferTemp);
    handleCommand();
}

std::vector<std::string> Socket::parseCommands(std::string &command) {
    std::vector<std::string> args;
    size_t pos = 0;

    while ((pos = command.find(' ')) != std::string::npos) {
        args.push_back(command.substr(0, pos));
        command.erase(0, pos + 1);
    }
    args.push_back(command);
    return args;
}

void Socket::sendDatasToServer(const std::string &message) const {
    if (fd.revents & POLLOUT) {
        const ssize_t bytes_sent = write(server_fd,
            message.c_str(), message.size());
        if (bytes_sent == -1) {
            throw std::runtime_error("Error sending data to server");
        }
    }
}

std::vector<std::vector<std::string>> Socket::getListOutputs() {
    std::vector<std::vector<std::string>> outputs;
    outputs.swap(listOuputs);
    return outputs;
}

}  // namespace Network
