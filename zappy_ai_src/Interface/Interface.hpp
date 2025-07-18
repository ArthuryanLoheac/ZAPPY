#pragma once

#include <queue>
#include <string>
#include <map>
#include <vector>

#include "Socket/Socket.hpp"

namespace AI {

/**
 * @brief Command constants for server communication
 */
#define FORWARD "Forward\n"
#define LEFT "Left\n"
#define RIGHT "Right\n"
#define LOOK "Look\n"
#define INVENTORY "Inventory\n"
#define CONNECT_NBR "Connect_nbr\n"
#define FORK "Fork\n"
#define EJECT "Eject\n"
#define INCANTATION "Incantation\n"

/**
 * @class Interface
 * @brief Main interface for AI communication with the Zappy server
 *
 * This class handles all communication between the AI client and the Zappy server.
 * It manages command sending/receiving, message processing, and game state updates.
 */
class Interface {
 public:
    /**
     * @brief Constructor for the Interface class
     */
    Interface();

    /**
     * @brief Destructor for the Interface class
     */
    ~Interface();

    /**
     * @brief Singleton instance getter
     * @return Reference to the singleton Interface instance
     */
    static Interface &i() {
        static Interface instance;
        return instance;
    }

    /**
     * @brief Initializes and starts the interface with server connection
     * @param port Server port to connect to
     * @param ip Server IP address
     * @param name AI player name to use for connection
     */
    void start(int port, const std::string &ip, const std::string &name);

    /**
     * @brief Main run loop that processes server communication
     * Handles incoming messages and processes command queues
     */
    void run();

    /**
     * @brief Stops the interface and disconnects from server
     */
    void stop();

    /**
     * @brief Sends a command to the server
     * @param command The command string to send
     */
    void sendCommand(const std::string &command);

    /**
     * @brief Return if the AI is waiting for a response from the server
     * @return true if waiting for a response, false otherwise
     */

    bool isWaitingForResponse() const;

    /**
     * @brief Moves the AI to a specific position on the map, based on the inventory direction system
     * @param x The x-coordinate to move to
     * @param y The y-coordinate to move to
     */
    void goTo(int x, int y);

    /**
     * @brief Turns the player around.
     */
    void doABarrelRoll();

    /**
     * @brief Moves the AI towards a specific direction by one tile,
     *  based on the AI's actual orientation.
     * @param direction Direction to move towards.
     *
     * Keep in mind that depending on the direction, 1 to 3 commands may be sent.
     */

    void goToDirection(int direction);

    /**
     * @brief Sends a message to the other players by using the broadcast command
     * @param message The message string to send
     */
    void sendMessage(const std::string &message);

 private:
    /**
     * @brief Processes the input and output queues to handle commands
     * Matches each input command with its corresponding output response
     */
    void handleQueues();

    /**
     * @brief Socket connection to the server
     */
    Network::Socket socket;

    /**
     * @brief Server port number
     */
    int port;

    /**
     * @brief Server IP address
     */
    std::string ip;

    /**
     * @brief AI player name
     */
    std::string name;

    /**
     * @brief Queue of commands sent to the server awaiting responses
     */
    std::queue<std::vector<std::string>> inputQueue;

    /**
     * @brief Queue of responses received from the server
     */
    std::queue<std::vector<std::string>> outputQueue;

    /**
     * @brief Map of command handlers indexed by command name
     * Maps command names to their handler methods
     */
    std::map<std::string, void(Interface::*)(std::vector<std::string> &,
        std::vector<std::string> &)> commands;

    /**
     * @brief Initializes the command handler map
     */
    void factoryCommands();

    /**
     * @brief Handles the WELCOME response from server
     * @param args Server response arguments
     * @param command Original command sent
     */
    void commandWELCOME(std::vector<std::string> &args,
        std::vector<std::string> &command);

    /**
     * @brief Handles the FORWARD command response
     * @param args Server response arguments
     * @param command Original command sent
     */
    void commandFORWARD(std::vector<std::string> &args,
        std::vector<std::string> &command);

    /**
     * @brief Handles the LEFT command response
     * @param args Server response arguments
     * @param command Original command sent
     */
    void commandLEFT(std::vector<std::string> &args,
        std::vector<std::string> &command);

    /**
     * @brief Handles the RIGHT command response
     * @param args Server response arguments
     * @param command Original command sent
     */
    void commandRIGHT(std::vector<std::string> &args,
        std::vector<std::string> &command);

    /**
     * @brief Handles the LOOK command response
     * Parses the vision data from the server
     * @param args Server response arguments
     * @param command Original command sent
     */
    void commandLOOK(std::vector<std::string> &args,
        std::vector<std::string> &command);

    /**
     * @brief Handles the INVENTORY command response
     * Updates the AI's inventory data
     * @param args Server response arguments
     * @param command Original command sent
     */
    void commandINVENTORY(std::vector<std::string> &args,
        std::vector<std::string> &command);

    /**
     * @brief Handles the TAKE command response
     * @param args Server response arguments
     * @param command Original command sent
     */
    void commandTAKE(std::vector<std::string> &args,
        std::vector<std::string> &command);

    /**
     * @brief Handles the SET command response
     * @param args Server response arguments
     * @param command Original command sent
     */
    void commandSET(std::vector<std::string> &args,
        std::vector<std::string> &command);

    /**
     * @brief Handles the EJECT command response
     * @param args Server response arguments
     * @param command Original command sent
     */
    void commandEJECT(std::vector<std::string> &args,
        std::vector<std::string> &command);

    /**
     * @brief Handles the FORK command response
     * Signals the parent process when a fork is successful
     * @param args Server response arguments
     * @param command Original command sent
     */
    void commandFORK(std::vector<std::string> &args,
        std::vector<std::string> &command);

    /**
     * @brief Handles the BROADCAST command response
     * @param args Server response arguments
     * @param command Original command sent
     */
    void commandBROADCAST(std::vector<std::string> &args,
        std::vector<std::string> &command);

    /**
     * @brief Handles the INCANTATION command response
     * Updates the AI's level if the incantation is successful
     * @param args Server response arguments
     * @param command Original command sent
     */
    void commandINCANTATION(std::vector<std::string> &args,
        std::vector<std::string> &command);

    /**
     * @brief Buffer for commands when the input queue is full
     */
    std::queue<std::string> commandBuffer;

    /**
     * @brief List of messages to filter out from broadcasts
     */
    std::vector<std::string> filteredStrings;

    /**
     * @brief Initializes the list of filtered broadcast messages
     */
    void initializeFilteredStrings();

    /**
     * @brief Checks if a message should be filtered
     * @param arg The message to check
     * @return true if the message should be filtered, false otherwise
     */
    bool needFilter(const std::string &arg);

    /**
     * @brief Processes received broadcast messages
     * @param args The message arguments including direction
     */
    void receiveMessage(const std::vector<std::string> &args);

    /**
     * @brief Handles the EJECT command response from the server
     * This method is called when the AI receives an EJECT command.
     * It processes the arguments and updates the AI's state accordingly.
     * @param args The arguments received from the server
     */
    void receiveEject(const std::vector<std::string> &args);

    /**
     * @brief Attempts to start an incantation ritual
     * @return true if the incantation started successfully, false otherwise
     */
    bool startIncantation();

    /**
     * @brief Base64 encoding map for the encryption/decryption functionality
     */
    static const char base64Chars[];

    /**
     * @brief Encode a string using Base64
     * @param input The binary data to encode
     * @return Base64 encoded string
     */
    static std::string base64Encode(const std::string& input);

    /**
     * @brief Decode a Base64 encoded string
     * @param encoded The Base64 encoded string
     * @return Decoded binary data
     */
    static std::string base64Decode(const std::string& encoded);

    /**
     * @brief Simple encryption function for command strings
     * @param input The input string to encrypt
     * @param key The encryption key
     * @return The encrypted string
     */
    static std::string encrypt(const std::string& input,
        const std::string& key);

    /**
     * @brief Simple decryption function for command strings
     * @param encrypted The encrypted string to decrypt
     * @param key The decryption key
     * @return The decrypted string
     */
    static std::string decrypt(const std::string& encrypted,
        const std::string& key);
};

}  // namespace AI
