#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <vector>

class pluginsData {
 public:
    int width; /**< Width of the game map. */
    int height; /**< Height of the game map. */
    int frequency; /**< Frequency of the game. */
    std::vector<std::string> teams; /**< List of team names. */
    std::vector<irr::video::SColor> teamColors; /**< List of team colors. */
};
