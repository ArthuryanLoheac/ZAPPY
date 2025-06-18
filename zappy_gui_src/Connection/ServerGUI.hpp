#pragma once
#include <poll.h>

#include <string>
#include <map>
#include <vector>
#include <chrono>

namespace GUI {

/**
 * @brief Manages the server communication and command handling for the GUI.
 */
class ServerGUI {
 public:
    int server_fd; /**< Server socket file descriptor. */
    int nb_fds; /**< Number of file descriptors. */
    struct pollfd fd; /**< Poll file descriptor structure. */
    std::string buffer; /**< Buffer for storing incoming data. */
    std::string outbuffer = ""; /**< Buffer for outgoing data. */
    int updateMapTime = 30; /**< Time interval for map updates. */
    int updatePingTime = 3; /**< Time interval for map updates. */
    int ping = 0; /**< Ping counter for server communication. */

    /**
     * @brief Constructs a new ServerGUI object.
     */
    ServerGUI();

    /**
     * @brief Provides a singleton instance of the ServerGUI class.
     *
     * @return ServerGUI& Reference to the singleton instance.
     */
    static ServerGUI &i() {
        static ServerGUI i;
        return i;
    }

    /**
     * @brief Sends data to the server.
     *
     * @param message The message to send.
     */
    void sendDatasToServer(const std::string &message);

    /**
     * @brief Starts the server communication loop.
     */
    void startServer();

    /**
     * @brief Get if the server is connected.
     */
    bool isConnectedToServer() const;

    /**
     * @brief Sets the server connection status.
     *
     * @param connected True if connected, false otherwise.
     */
    void setConnectedToServer(bool connected);

 private:
    bool sendPing = false; /**< Flag to indicate if BCT command has been sent */
    std::chrono::_V2::system_clock::time_point timeForPing;
    bool isConnected = false; /**< if the server connection is active */

    /**
     * @brief Updates the clock
     *
     * @param time Current time point.
     * @param timeNext Next scheduled time point.
     */
    void clockUpdate(std::chrono::system_clock::time_point &time,
        std::chrono::system_clock::time_point &timeNext,
        std::chrono::system_clock::time_point &timeNextPing);

    /**
     * @brief Handles parsing and executing commands received from the server.
     */
    void handleCommand();

    /**
     * @brief Reads data from the server.
     */
    void readDatasFromServer();

    /**
     * @brief Parses commands received from the server.
     *
     * @param command The raw command string.
     * @return std::vector<std::string> Parsed command arguments.
     */
    std::vector<std::string> parseCommands(std::string &command);

    /**
     * @brief Executes a specific command based on the parsed input.
     *
     * @param it Iterator pointing to the command in the command map.
     * @param args Arguments for the command.
     */
    void execCommand(std::map<std::string, void(GUI::ServerGUI::*)
    (std::vector<std::string> &)>::iterator it, std::vector<std::string> &args);

    /**
     * @brief Handles the "WELCOME" command from the server.
     *
     * @param args Command arguments (unused).
     */
    void welcomeCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "MSZ" command to set map dimensions.
     *
     * @param args Command arguments containing width and height.
     */
    void mszCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "ENW" command to add an egg.
     *
     * @param args Command arguments containing egg details.
     */
    void enwCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "TNA" command to add a team name.
     *
     * @param args Command arguments containing the team name.
     */
    void tnaCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "BCT" command to update tile resources.
     *
     * @param args Command arguments containing tile coordinates and resources.
     */
    void bctCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "SGT" command to set the game frequency.
     *
     * @param args Command arguments containing the frequency.
     */
    void sgtCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "EBO" command to remove an egg.
     *
     * @param args Command arguments containing the egg ID.
     */
    void eboCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "EDI" command to remove an egg due to death.
     *
     * @param args Command arguments containing the egg ID.
     */
    void ediCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "PNW" command to add a new player.
     *
     * @param args Command arguments containing player details.
     */
    void pnwCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "PPO" command to update a player's position.
     *
     * @param args Command arguments containing player ID, position, and orientation.
     */
    void ppoCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "PIN" command to update a player's inventory and position.
     *
     * @param args Command arguments containing player ID, position, and resources.
     */
    void pinCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "PDI" command to remove a player due to death.
     *
     * @param args Command arguments containing the player ID.
     */
    void pdiCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "PLV" command to update a player's level.
     *
     * @param args Command arguments containing player ID and level.
     */
    void plvCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "PIC" command to start an elevation
     *
     * @param args Command arguments containing player ID and resources.
     */
    void picCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "PIE" command to finish an elevation.
     *
     * @param args Command arguments containing player ID and resources.
     */
    void pieCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "PDR" command to take ressource from a tile
     *
     * @param args Command arguments containing player ID.
     */
    void pdrCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "PGT" command to get the tile resources.
     *
     * @param args Command arguments containing player ID and tile coordinates.
     */
    void pgtCommand(std::vector<std::string> &args);

    /**
     * @brief Handles the "PEX" command to notify that a player has been ejected.
     *
     * @param args Command arguments containing player ID.
     */
    void pexCommand(std::vector<std::string> &args);

    void sucCommand(std::vector<std::string> &args);

    /**
     * @brief Map of server commands to their corresponding handler functions.
     */
    std::map<std::string,
     void(ServerGUI::*)(std::vector<std::string> &)> commands = {
        {"WELCOME", &ServerGUI::welcomeCommand},
        {"MSZ", &ServerGUI::mszCommand},
        {"ENW", &ServerGUI::enwCommand},
        {"TNA", &ServerGUI::tnaCommand},
        {"BCT", &ServerGUI::bctCommand},
        {"SGT", &ServerGUI::sgtCommand},
        {"SST", &ServerGUI::sgtCommand},
        {"EBO", &ServerGUI::eboCommand},
        {"EDI", &ServerGUI::ediCommand},
        {"PNW", &ServerGUI::pnwCommand},
        {"PPO", &ServerGUI::ppoCommand},
        {"PIN", &ServerGUI::pinCommand},
        {"PDI", &ServerGUI::pdiCommand},
        {"PLV", &ServerGUI::plvCommand},
        {"PIC", &ServerGUI::picCommand},
        {"PIE", &ServerGUI::pieCommand},
        {"PDR", &ServerGUI::pdrCommand},
        {"PGT", &ServerGUI::pgtCommand},
        {"PEX", &ServerGUI::pexCommand},
        {"SUC", &ServerGUI::sucCommand},

    };
};

}  // namespace GUI
