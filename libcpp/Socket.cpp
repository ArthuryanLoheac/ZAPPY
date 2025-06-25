#include "Socket/Socket.hpp"

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

namespace Network {

/**
 * @brief Default constructor
 *
 * Initializes all members to default values.
 */
Socket::Socket(): port(0), ip(), server_fd(0), nb_fds(0), fd(), running(false) {
}

/**
 * @brief Parameterized constructor
 *
 * Initializes the socket with given port and IP address.
 * @param port Port number to be used for connection
 * @param ip IP address to be used for connection
 */
Socket::Socket(const int port, const std::string &ip) : port(port), ip(ip),
server_fd(0), nb_fds(0), fd(), running(false) {}

/**
 * @brief Destructor
 *
 * Ensures the socket is stopped if it's still running.
 */
Socket::~Socket() {
    if (running) {
        stopSocket();
    }
}

/**
 * @brief Starts the socket with specified port and IP
 *
 * @param port Port number to connect to
 * @param ip IP address to connect to
 * @throws std::runtime_error if socket is already running
 */
void Socket::startSocket(const int port, const std::string &ip) {
    this->port = port;
    this->ip = ip;
    if (running)
        throw std::runtime_error("Socket is already running");
    startSocket();
}

/**
 * @brief Starts the socket with previously set port and IP
 *
 * Creates a socket, connects to the server, and sets up polling.
 * @throws std::runtime_error if socket is already running or connection fails
 */
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

/**
 * @brief Polls for socket events
 *
 * Waits for events on the socket and reads data if available.
 * @throws std::runtime_error if polling fails
 */
void Socket::run() {
    if (!running) {
        throw std::runtime_error("Socket is not running");
    }
    if (poll(&fd, 1, -1) == -1)
        throw std::runtime_error("Poll error occurred");
    if (fd.revents & POLLIN)
        readDatasFromServer();
}

/**
 * @brief Stops the socket
 *
 * Closes the connection and resets the socket state.
 */
void Socket::stopSocket() {
    if (running) {
        close(server_fd);
        running = false;
        server_fd = 0;
        buffer.clear();
    }
}

/**
 * @brief Processes commands in the buffer
 *
 * Extracts commands from buffer that end with newline characters,
 * parses them, and adds them to the output list.
 */
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

/**
 * @brief Reads data from server into buffer
 *
 * @throws std::runtime_error if reading from server fails
 */
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

/**
 * @brief Parses a command string into arguments
 *
 * Splits a command string by spaces and returns each component as a vector element.
 * @param command The command string to parse
 * @return Vector of parsed arguments
 */
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

/**
 * @brief Sends data to the server
 *
 * @param message The message to send
 * @throws std::runtime_error if sending data fails
 */
void Socket::sendDatasToServer(const std::string &message) {
    writeBuffer += message;
}

void Socket::flushWriteBuffer() {
    while (!writeBuffer.empty()) {
        ssize_t bytes_sent = write(server_fd, writeBuffer.c_str(),
            writeBuffer.size());
        if (bytes_sent == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            throw std::runtime_error("Error sending data to server");
        }
        writeBuffer.erase(0, bytes_sent);
    }
}

/**
 * @brief Gets and clears the list of command outputs
 *
 * @return Vector of command arguments
 */
std::vector<std::vector<std::string>> Socket::getListOutputs() {
    std::vector<std::vector<std::string>> outputs;
    outputs.swap(listOuputs);
    return outputs;
}

}  // namespace Network
