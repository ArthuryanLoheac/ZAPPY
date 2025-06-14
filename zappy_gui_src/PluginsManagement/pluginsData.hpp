#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <vector>

class pluginsData {
 public:
    class Tile {
     public:
        int x; /**< X-coordinate of the tile. */
        int y; /**< Y-coordinate of the tile. */
        std::vector<int> resources; /**< List of resources on the tile. */
    };

    int width; /**< Width of the game map. */
    int height; /**< Height of the game map. */
    int frequency; /**< Frequency of the game. */
    std::vector<std::string> teams; /**< List of team names. */
    std::vector<irr::video::SColor> teamColors; /**< List of team colors. */
};
