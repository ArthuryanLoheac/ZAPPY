#pragma once
#include <mutex>
#include <vector>
#include <stdexcept>
#include <string>
#include <memory>

#include "DataManager/GameTile.hpp"
#include "DataManager/Player.hpp"
#include "DataManager/Egg.hpp"

namespace GUI {

/**
 * @brief Manages game data, including tiles, players, eggs, and teams.
 */
class GameDataManager {
 private:
    int width; /**< Width of the game map. */
    int height; /**< Height of the game map. */
    std::vector<GameTile> tiles; /**< List of game tiles. */
    std::vector<Egg> eggs; /**< List of eggs. */
    std::vector<std::string> teams; /**< List of team names. */
    std::vector<Player> players; /**< List of players. */

 public:
    std::mutex mutexDatas; /**< Mutex for thread-safe access. */

    /**
     * @brief Provides a singleton instance of the GameDataManager class.
     * @return GameDataManager& Reference to the singleton instance.
     */
    static GameDataManager &i() {
        static GameDataManager i;
        return i;
    }

    /**
     * @brief Constructs a new GameDataManager object.
     */
    GameDataManager() : width(-1), height(-1) {}

    /**
     * @brief Gets the width of the game map.
     * @return int Width of the game map.
     */
    int getWidth() const;

    /**
     * @brief Gets the height of the game map.
     * @return int Height of the game map.
     */
    int getHeight() const;

    /**
     * @brief Sets the width of the game map.
     * @param w Width to set.
     */
    void setWidth(int w);

    /**
     * @brief Sets the height of the game map.
     * @param h Height to set.
     */
    void setHeight(int h);

    /**
     * @brief Adds a tile to the game map.
     * @param x X-coordinate of the tile.
     * @param y Y-coordinate of the tile.
     * @return GameTile& Reference to the added tile.
     */
    GameTile &addTile(int x, int y);

    /**
     * @brief Gets a tile from the game map.
     * @param x X-coordinate of the tile.
     * @param y Y-coordinate of the tile.
     * @return GameTile& Reference to the requested tile.
     */
    GameTile &getTile(int x, int y);

    /**
     * @brief Gets the list of eggs.
     * @return const std::vector<Egg>& Reference to the list of eggs.
     */
    const std::vector<Egg> &getEggs() const;

    /**
     * @brief Adds an egg to the game map.
     *
     * @param id ID of the egg.
     * @param team Team of the egg.
     * @param x X-coordinate of the egg.
     * @param y Y-coordinate of the egg.
     */
    void addEgg(int id, int team, int x, int y);

    /**
     * @brief Removes an egg from the game map.
     * @param id ID of the egg to remove.
     */
    void removeEgg(int id);

    /**
     * @brief Adds a player to the game map.
     *
     * @param id ID of the player.
     * @param x X-coordinate of the player.
     * @param y Y-coordinate of the player.
     * @param o Orientation of the player.
     * @param level Level of the player.
     * @param teamName Name of the player's team.
     */
    void addPlayer(int id, int x, int y,
        Player::Orientation o, int level, const std::string &teamName);

    /**
     * @brief Gets a player by ID.
     *
     * @param id ID of the player.
     * @return Player& Reference to the requested player.
     */
    Player &getPlayer(int id);

    /**
     * @brief Gets the list of players.
     *
     * @return const std::vector<Player>& Reference to the list of players.
     */
    std::vector<Player> &getPlayers();

    /**
     * @brief Removes a player from the game map.
     *
     * @param id ID of the player to remove.
     */
    void removePlayer(int id);

    /**
     * @brief Adds a team to the game.
     *
     * @param teamName Name of the team to add.
     */
    void addTeam(const std::string &teamName);

    /**
     * @brief Gets the list of teams.
     *
     * @return const std::vector<std::string>& Reference to the list of teams.
     */
    const std::vector<std::string> &getTeams() const;

    /**
     * @brief Updates the game data.
     *
     * @param deltaTime Time elapsed since the last update.
     */
    void Update(float deltaTime);
};

}  // namespace GUI
