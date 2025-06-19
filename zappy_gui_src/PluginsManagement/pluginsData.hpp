#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <vector>
#include <stdexcept>


class pluginsData {
/** @brief Class representing the data structure
 * for the game to send to the plugins*/
 public:
    class Tile {
        /** @brief Represents a tile in the game map,
         * including its coordinates and resources. */
     public:
        int x; /**< X-coordinate of the tile. */
        int y; /**< Y-coordinate of the tile. */
        std::vector<int> resources; /**< List of resources on the tile. */
    };

    class Player {
     /** @brief Represents a player in the game, including position,
      * orientation, level, and resources. */
     public:
        enum class Orientation {
            NORTH,
            SOUTH,
            EAST,
            WEST
        } orientation; /**< Orientation of the player. */

        int id; /**< ID of the player. */
        std::string teamName; /**< Name of the player's team. */
        int x; /**< X-coordinate of the player. */
        int y; /**< Y-coordinate of the player. */
        int level; /**< Level of the player. */
        irr::video::SColor color; /**< Color associated with the player. */
        irr::core::vector3df position;
             /**< Position of the player in 3D space. */
        std::vector<int> ressources;
             /**< List of resources held by the player. */
        std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> PlayerMesh;
            /**< Mesh associated with the player. */
    };

    int width; /**< Width of the game map. */
    int height; /**< Height of the game map. */
    int frequency; /**< Frequency of the game. */
    std::vector<std::string> teams; /**< List of team names. */
    std::vector<irr::video::SColor> teamColors; /**< List of team colors. */
    std::vector<Tile> tiles; /**< List of tiles in the game map. */
    std::vector<Player> players; /**< List of players in the game. */
    int ping = 0; /**< Ping time to the server. */

    std::string winner; /**< Name of the winning team. */
    bool isGameOver = false; /**< Flag indicating if the game is over. */

    bool isConnected = false; /**< Connection status to the server. */

    /**
     * @brief Get the player by ID.
     *
     * @param id ID of the player to retrieve.
     * @return Player object corresponding to the given ID.
     * @throws std::out_of_range if the player with the given ID is not found.
     */
    Player getPlayer(int id) const {
        for (const auto &player : players) {
            if (player.id == id)
                return player;
        }
        throw std::out_of_range("Player not found");
    }
};
