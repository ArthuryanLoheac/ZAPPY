#pragma once

#include <poll.h>

#include <string>
#include <map>
#include <vector>

namespace Network {

/**
 * @class Socket
 * @brief A socket wrapper class for network communication
 *
 * The Socket class provides functionality for creating a socket connection,
 * sending and receiving data, and handling commands from a server.
 */
class Socket {
 public:
    int port;                   ///< Port number for the socket connection
    std::string ip;             ///< IP address for the socket connection
    int server_fd;              ///< Server file descriptor
    int nb_fds;                 ///< Number of file descriptors
    struct pollfd fd;           ///< Poll file descriptor structure
    bool running;               ///< Flag indicating if the socket is running
    std::string buffer;         ///< Buffer for incoming data

    /**
     * @brief Default constructor
     *
     * Initializes a socket with default values
     */
    Socket();

    /**
     * @brief Parameterized constructor
     *
     * @param port The port number to connect to
     * @param ip The IP address to connect to (defaults to "localhost")
     */
    explicit Socket(int port, const std::string &ip = "localhost");

    /**
     * @brief Destructor
     *
     * Stops the socket if it's running
     */
    ~Socket();

    /**
     * @brief Singleton instance getter
     *
     * @return Reference to the singleton instance of Socket
     */
    static Socket &i() {
        static Socket i;
        return i;
    }

    /**
     * @brief Start the socket with previously set port and IP
     *
     * @throws std::runtime_error If the socket is already running or if connection fails
     */
    void startSocket();

    /**
     * @brief Start the socket with specified port and IP
     *
     * @param port The port number to connect to
     * @param ip The IP address to connect to (defaults to "localhost")
     * @throws std::runtime_error If the socket is already running or if connection fails
     */
    void startSocket(int port, const std::string &ip = "localhost");

    /**
     * @brief Run the socket polling mechanism
     *
     * Polls for socket events and reads data if available
     * @throws std::runtime_error If poll error occurs
     */
    void run();

    /**
     * @brief Stop the socket
     *
     * Closes the connection and resets socket variables
     */
    void stopSocket();

    /**
     * @brief Send data to the server
     *
     * @param message The message to send
     * @throws std::runtime_error If sending data fails
     */
    void sendDatasToServer(const std::string &message);

    /**
     * @brief Get the list of processed commands
     *
     * @return Vector of command arguments, where each command is a vector of strings
     */
    std::vector<std::vector<std::string>> getListOutputs();

    /**
     * @brief Process the buffer for commands
     *
     * Parses the buffer for commands separated by newlines and processes them
     */
    void handleCommand();

 private:
    /**
     * @brief Read data from the server into the buffer
     *
     * @throws std::runtime_error If reading from server fails
     */
    void readDatasFromServer();

    /**
     * @brief Parse a command string into arguments
     *
     * @param command The command string to parse
     * @return Vector of parsed arguments
     */
    static std::vector<std::string> parseCommands(std::string &command);

    /// List of processed command outputs
    std::vector<std::vector<std::string>> listOuputs;
    std::string writeBuffer;
    void flushWriteBuffer();
};

}  // namespace Network
