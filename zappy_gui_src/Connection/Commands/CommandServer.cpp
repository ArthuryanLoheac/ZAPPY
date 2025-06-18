#include <iostream>
#include <string>
#include <vector>

#include "Connection/ServerGUI.hpp"
#include "Exceptions/ServerGUIExceptions.hpp"
#include "Window/window.hpp"
#include "DataManager/GameDataManager.hpp"
#include "DataManager/DataManager.hpp"
#include "DataManager/SoundsManager.hpp"

#include "include/logs.h"

/**
 * @brief Prints an error message and the associated arguments.
 * @param e The exception that was thrown.
 * @param args The arguments related to the error.
 */
void printError(const std::exception &e, std::vector<std::string> &args) {
    std::cerr << "Error: " << e.what() << std::endl;
    std::cerr << "Args: ";
    for (const auto &arg : args)
        std::cerr << arg << " ";
    std::cerr << std::endl;
}

namespace GUI {

/**
 * @brief Sends the welcome command to the server.
 * @param args Command arguments (unused).
 */
void GUI::ServerGUI::welcomeCommand(std::vector<std::string> &args) {
    (void) args;
    sendDatasToServer("GRAPHIC\n");
}

/**
 * @brief Processes the "msz" command to set the map dimensions.
 * @param args Command arguments containing width and height.
 * @throws CommandParsingException If the command format or dimensions are invalid.
 */
void ServerGUI::mszCommand(std::vector<std::string> &args) {
    if (args.size() != 3)
        throw CommandParsingException("Invalid msz command format");
    int width = std::stoi(args[1]);
    int height = std::stoi(args[2]);
    if (width <= 0 || height <= 0)
        throw CommandParsingException("Invalid dimensions in msz command");
    GameDataManager::i().setWidth(width);
    GameDataManager::i().setHeight(height);

    GUI::Window::i().setupWorld();
}

/**
 * @brief Processes the "enw" command to add an egg to the game.
 * @param args Command arguments containing egg details.
 * @throws CommandParsingException If the command format or details are invalid.
 */
void GUI::ServerGUI::enwCommand(std::vector<std::string> &args) {
    if (args.size() != 5)
        throw CommandParsingException("Invalid enw command format");
    if (args[1].size() < 2 || args[2].size() < 2)
        throw CommandParsingException("Invalid team or id name");
    int id = std::stoi(args[1].substr(1));
    int team = std::stoi(args[2].substr(1));
    int x = std::stoi(args[3]);
    int y = std::stoi(args[4]);

    GameDataManager::i().addEgg(id, team, x, y);
    if (team >= 0)
        GameDataManager::i().setEggAdded(true);
    GUI::Window::i().needUpdateEggs = true;
}

/**
 * @brief Processes the "tna" command to add a team name.
 * @param args Command arguments containing the team name.
 * @throws CommandParsingException If the command format is invalid.
 */
void ServerGUI::tnaCommand(std::vector<std::string> &args) {
    if (args.size() != 2)
        throw CommandParsingException("Invalid tna command format");

    std::string teamName = args[1];
    GameDataManager::i().addTeam(teamName);
}

/**
 * @brief Processes the "bct" command to update tile resources.
 * @param args Command arguments containing tile coordinates and resources.
 * @throws CommandParsingException If the command format is invalid.
 */
void ServerGUI::bctCommand(std::vector<std::string> &args) {
    if (args.size() != 10)
        throw CommandParsingException("Invalid bct command format");
    int x = std::stoi(args[1]);
    int y = std::stoi(args[2]);
    int food = std::stoi(args[3]);
    int r1 = std::stoi(args[4]);
    int r2 = std::stoi(args[5]);
    int r3 = std::stoi(args[6]);
    int r4 = std::stoi(args[7]);
    int r5 = std::stoi(args[8]);
    int r6 = std::stoi(args[9]);

    GameDataManager::i().getTile(x, y).setRessources(food, r1, r2, r3,
        r4, r5, r6);
    GUI::Window::i().needUpdateRessources = true;
}

/**
 * @brief Processes the "sgt" command to set the game frequency.
 * @param args Command arguments containing the frequency.
 * @throws CommandParsingException If the command format is invalid.
 */
void ServerGUI::sgtCommand(std::vector<std::string> &args) {
    if (args.size() != 2)
        throw CommandParsingException("Invalid sgt command format");
    int f = std::stoi(args[1]);

    DataManager::i().setFrequency(f);
}

/**
 * @brief Processes the "ebo" command to remove an egg.
 * @param args Command arguments containing the egg ID.
 * @throws CommandParsingException If the command format or ID is invalid.
 */
void ServerGUI::eboCommand(std::vector<std::string> &args) {
    if (args.size() != 2)
        throw CommandParsingException("Invalid ebo command format");
    if (args[1].size() < 2)
        throw CommandParsingException("Invalid id name");
    int id = std::stoi(args[1].substr(1));

    GameDataManager::i().removeEgg(id);
}

/**
 * @brief Processes the "edi" command to remove an egg due to death.
 * @param args Command arguments containing the egg ID.
 * @throws CommandParsingException If the command format or ID is invalid.
 */
void ServerGUI::ediCommand(std::vector<std::string> &args) {
    if (args.size() != 2)
        throw CommandParsingException("Invalid edi command format");
    if (args[1].size() < 2)
        throw CommandParsingException("Invalid id name");
    int id = std::stoi(args[1].substr(1));

    GameDataManager::i().removeEgg(id);
    GameDataManager::i().setEggDead(true);
}

/**
 * @brief Processes the "pnw" command to add a new player.
 * @param args Command arguments containing player details.
 * @throws CommandParsingException If the command format or details are invalid.
 */
void ServerGUI::pnwCommand(std::vector<std::string> &args) {
    if (args.size() != 7)
        throw CommandParsingException("Invalid pnw command format");
    if (args[1].size() < 2)
        throw CommandParsingException("Invalid id name");
    int id = std::stoi(args[1].substr(1));
    int x = std::stoi(args[2]);
    int y = std::stoi(args[3]);
    int orient = std::stoi(args[4]) - 1;
    int level = std::stoi(args[5]);
    std::string teamName = args[6];
    if (orient < 0 || orient > 3)
        throw CommandParsingException("Invalid orientation in pnw command");
    if (level < 1 || level > 8)
        throw CommandParsingException("Invalid level in pnw command");
    Player::Orientation pOrient;

    if (orient == 0)
        pOrient = Player::WEST;
    else if (orient == 1)
        pOrient = Player::NORTH;
    else if (orient == 2)
        pOrient = Player::EAST;
    else
        pOrient = Player::SOUTH;

    GameDataManager::i().addPlayer(id, x, y, pOrient, level, teamName);
    GUI::Window::i().setUpdatePlayer(true);
}

/**
 * @brief Processes the "ppo" command to update a player's position.
 * @param args Command arguments containing player ID, position, and orientation.
 * @throws CommandParsingException If the command format or details are invalid.
 */
void ServerGUI::ppoCommand(std::vector<std::string> &args) {
    if (args.size() != 5)
        throw CommandParsingException("Invalid ppo command format");
    if (args[1].size() < 2)
        throw CommandParsingException("Invalid id name");
    int id = std::stoi(args[1].substr(1));
    int x = std::stoi(args[2]);
    int y = std::stoi(args[3]);
    int orient = std::stoi(args[4]) - 1;
    if (orient < 0 || orient > 3)
        throw CommandParsingException("Invalid orientation in ppo command");
    Player::Orientation pOrient;

    if (orient == 0)
        pOrient = Player::WEST;
    else if (orient == 1)
        pOrient = Player::NORTH;
    else if (orient == 2)
        pOrient = Player::EAST;
    else
        pOrient = Player::SOUTH;

    GameDataManager::i().getPlayer(id).setPosition(x, y, pOrient);
}

/**
 * @brief Processes the "pin" command to update a player's inventory and position.
 * @param args Command arguments containing player ID, position, and resources.
 * @throws CommandParsingException If the command format or details are invalid.
 */
void ServerGUI::pinCommand(std::vector<std::string> &args) {
    if (args.size() != 11)
        throw CommandParsingException("Invalid pin command format");
    if (args[1].size() < 2)
        throw CommandParsingException("Invalid id name");
    int id = std::stoi(args[1].substr(1));
    int x = std::stoi(args[2]);
    int y = std::stoi(args[3]);

    Player &p = GameDataManager::i().getPlayer(id);
    for (int i = 4; i <= 10; i++) {
        int r = std::stoi(args[i]);
        if (r < 0)
            throw CommandParsingException("Invalid resource in pin command");
        p.setRessource(i - 4, r);
    }
    p.setPosition(x, y);
}

/**
 * @brief Processes the "pdi" command to remove a player due to death.
 * @param args Command arguments containing the player ID.
 * @throws CommandParsingException If the command format or ID is invalid.
 */
void ServerGUI::pdiCommand(std::vector<std::string> &args) {
    if (args.size() != 2)
        throw CommandParsingException("Invalid pdi command format");
    if (args[1].size() < 2)
        throw CommandParsingException("Invalid id name");
    int id = std::stoi(args[1].substr(1));
    GameDataManager::i().removePlayer(id);
    GameDataManager::i().setPlayerDead(true);
}

/**
 * @brief Processes the "plv" command to update a player's level.
 * @param args Command arguments containing player ID and level.
 * @throws CommandParsingException If the command format or level is invalid.
 */
void ServerGUI::plvCommand(std::vector<std::string> &args) {
    if (args.size() != 3)
        throw CommandParsingException("Invalid plv command format");
    if (args[1].size() < 2)
        throw CommandParsingException("Invalid id name");
    int id = std::stoi(args[1].substr(1));
    int level = std::stoi(args[2]);
    if (level < 1 || level > 8)
        throw CommandParsingException("Invalid level in plv command");

    GameDataManager::i().getPlayer(id).setLevel(level);
}

void ServerGUI::picCommand(std::vector<std::string> &args) {
    if (args.size() < 5)
        throw CommandParsingException("Invalid pic command format");
    for (size_t i = 4; i < args.size(); i++) {
        if (args[i].size() < 2)
            throw CommandParsingException("Invalid id name in pic command");
        int id = std::stoi(args[i].substr(1));
        int x = std::stoi(args[1]);
        int y = std::stoi(args[2]);
        GameDataManager::i().getPlayer(id).setPosition(x, y,
            GameDataManager::i().getPlayer(id).getOrientation(), true);
        GameDataManager::i().getPlayer(id).setElevation(true);
        GameDataManager::i().setElevationSound(true);
    }
}

void ServerGUI::pieCommand(std::vector<std::string> &args) {
    if (args.size() < 4)
        throw CommandParsingException("Invalid pie command format");
    int x = std::stoi(args[1]);
    int y = std::stoi(args[2]);
    std::string result = "";
    for (size_t i = 3; i < args.size(); i++)
        result += args[i] + " ";
    for (auto &player : GameDataManager::i().getPlayers()) {
        if (player.getX() == x && player.getY() == y)
            player.setElevation(false);
    }
    GameDataManager::i().setElevationSound(true);
}

void ServerGUI::pdrCommand(std::vector<std::string> &args) {
    if (args.size() != 3)
        throw CommandParsingException("Invalid pdr command format");
    if (args[1].size() < 2)
        throw CommandParsingException("Invalid id name");
    GameDataManager::i().setDropping(true);
}

void ServerGUI::pgtCommand(std::vector<std::string> &args) {
    if (args.size() != 3)
        throw CommandParsingException("Invalid pgt command format");
    if (args[1].size() < 2)
        throw CommandParsingException("Invalid id name");
    GameDataManager::i().setCollecting(true);
}

void ServerGUI::pexCommand(std::vector<std::string> &args) {
    if (args.size() != 2)
        throw CommandParsingException("Invalid pex command format");
    if (args[1].size() < 2)
        throw CommandParsingException("Invalid id name");
    GameDataManager::i().setPushed(true);
}
}  // namespace GUI

void GUI::ServerGUI::sucCommand(std::vector<std::string> &args) {
    (void) args;
    if (sendPing) {
        ping = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - timeForPing).count();
    }
}
