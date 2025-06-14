#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <vector>
#include <stdexcept>


class pluginsData {
 public:
    class Tile {
     public:
        int x; /**< X-coordinate of the tile. */
        int y; /**< Y-coordinate of the tile. */
        std::vector<int> resources; /**< List of resources on the tile. */
    };

    class Player {
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
        irr::core::vector3df position; /**< Position of the player in 3D space. */
        std::vector<int> ressources; /**< List of resources held by the player. */
        std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> PlayerMesh; /**< Mesh associated with the player. */
    };

    int width; /**< Width of the game map. */
    int height; /**< Height of the game map. */
    int frequency; /**< Frequency of the game. */
    std::vector<std::string> teams; /**< List of team names. */
    std::vector<irr::video::SColor> teamColors; /**< List of team colors. */
    std::vector<Tile> tiles; /**< List of tiles in the game map. */
    std::vector<Player> players; /**< List of players in the game. */

    Player getPlayer(int id) const {
        for (const auto &player : players) {
            if (player.id == id)
                return player;
        }
        throw std::out_of_range("Player not found");
    }
};
