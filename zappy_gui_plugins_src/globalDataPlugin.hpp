#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/include/Aplugin.hpp"

/**
 * @class globalDataPlugin
 * @brief Plugin class for managing global data visualization and interaction.
 * @implements Aplugin
 */
class globalDataPlugin : public Aplugin {
 public:
    /**
     * @brief Draws the UI elements for the plugin.
     * @param font Shared pointer to the GUI font.
     * @param driver Pointer to the video driver.
     */
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;

    std::string getName() const override {
        return "Global Data Plugin";
    }

    /**
     * @brief Count the number of players on a specific team.
     * @param teamName The name of the team.
     * @return The number of players on the team.
     */
    int countNbPlayersOnTeam(const std::string &teamName);

    /**
     * @brief Count the number of players on a specific team at a given level.
     * @param teamName The name of the team.
     * @param level The level to check.
     * @return The number of players on the team at the specified level.
     */
    int countNbPlayersOnTeam(const std::string &teamName, int level);

    /**
     * @brief Count the number of different levels for players on a team.
     * @param teamName The name of the team.
     * @return The number of different levels for players on the team.
     */
    int countNbLevelDifferentTeams(const std::string &teamName);
};
