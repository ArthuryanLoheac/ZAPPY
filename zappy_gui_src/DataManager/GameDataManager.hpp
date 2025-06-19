#pragma once
#include <mutex>
#include <vector>
#include <stdexcept>
#include <string>
#include <memory>
#include <queue>

#include "DataManager/GameTile.hpp"
#include "DataManager/Player.hpp"
#include "DataManager/Egg.hpp"

namespace GUI {

/**
 * @brief Manages game data, including tiles, players, eggs, and teams.
 */
class GameDataManager {
 private:
    class Message {
        /** @brief Represents a message in the game,
         * including its content and associated player ID. */
     public:
        Message(const std::string &content, int playerId)
            : content(content), playerId(playerId) {}
        std::string content; /**< Content of the message. */
        int playerId; /**< ID of the player who sent the message. */
    };

    int width; /**< Width of the game map. */
    int height; /**< Height of the game map. */
    std::vector<GameTile> tiles; /**< List of game tiles. */
    std::vector<Egg> eggs; /**< List of eggs. */
    std::vector<std::string> teams; /**< List of team names. */
    std::vector<Player> players; /**< List of players. */
    bool playerAdded = false; /**< Flag to indicate if a player has been add */
    bool eggAdded = false; /**< Flag to indicate if an egg has been added. */
    bool playerDead = false; /**< Flag to indicate if a player has died. */
    bool eggDead = false; /**< Flag to indicate if an egg has died. */
    bool elevation = false; /**< Flag to indicate if elevation is active. */
    bool Collecting = false; /**< Flag to indicate if collecting is active. */
    bool Dropping = false; /**< Flag to indicate if dropping is active. */
    bool Pushed = false; /**< Flag to indicate if a player has been pushed. */
    std::vector<Message> messages;
    std::vector<Message> messagesThisFrame;

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
    std::vector<Egg> &getEggs();

    /**
     * @brief Adds a message to the message queue.
     * @param message The message to add.
     */
    void addMessage(const std::string &message, int id);

    /**
     * @brief Gets the list of messages.
     * @return std::queue<std::string>& Reference to the message queue.
     */
    std::vector<Message> &getMessages();

    /**
     * @brief Gets the list of messages.
     * @return std::queue<std::string>& Reference to the message queue.
     */
    std::vector<Message> &getMessagesThisFrame();

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
     * @brief Checks if a player has been added to the game map.
     * @return bool True if a player has been added, false otherwise.
     */
    bool isPlayerAdded() const;

    /**
     * @brief Sets the player added flag.
     *
     * @param added True if a player has been added, false otherwise.
     */
    void setPlayerAdded(bool added);

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

    /**
     * @brief Checks if an egg has been added.
     * @return bool True if an egg has been added, false otherwise.
     */
    bool isEggAdded() const;

    /**
     * @brief Sets the egg added flag.
     *
     * @param added True if an egg has been added, false otherwise.
     */
    void setEggAdded(bool added);

    /**
     * @brief Checks if a player has died.
     * @return bool True if a player has died, false otherwise.
     */
    bool isPlayerDead() const;

    /**
     * @brief Sets the player dead flag.
     *
     * @param dead True if a player has died, false otherwise.
     */
    void setPlayerDead(bool dead);

    /**
     * @brief Checks if an egg has died.
     * @return bool True if an egg has died, false otherwise.
     */
    bool isEggDead() const;

    /**
     * @brief Sets the egg dead flag.
     *
     * @param dead True if an egg has died, false otherwise.
     */
    void setEggDead(bool dead);

    /**
     * @brief Checks if elevation is active.
     * @return bool True if elevation is active, false otherwise.
     */
    bool isElevation() const;

    /**
     * @brief Sets the elevation flag.
     *
     * @param elev True if elevation is active, false otherwise.
     */
    void setElevationSound(bool elev);

    /**
     * @brief Checks if collecting is active.
     * @return bool True if collecting is active, false otherwise.
     */
    bool isCollecting() const;

    /**
     * @brief Sets the collecting flag.
     *
     * @param collecting True if collecting is active, false otherwise.
     */
    void setCollecting(bool collecting);

    /**
     * @brief Checks if dropping is active.
     * @return bool True if dropping is active, false otherwise.
     */
    bool isDropping() const;

    /**
     * @brief Sets the dropping flag.
     *
     * @param dropping True if dropping is active, false otherwise.
     */
    void setDropping(bool dropping);

    /**
     * @brief Checks if a player has been pushed.
     * @return bool True if a player has been pushed, false otherwise.
     */
    bool isPushed() const;

    /**
     * @brief Sets the pushed flag.
     *
     * @param pushed True if a player has been pushed, false otherwise.
     */
    void setPushed(bool pushed);
};

}  // namespace GUI
