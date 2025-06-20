#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/include/pluginA.hpp"

/**
 * @class PlayerDataPlugin
 * @brief Plugin class for managing player data visualization and interaction
 * when clicking on one
 * @implements pluginA
 */
class PlayerDataPlugin : public pluginA {
 private:
    int idPlayer = -1; /**< ID of the selected player. */

    bool pressed = false; /**< Indicates if the mouse is currently pressed. */
    bool isPressedLastFrame = false;
      /**< Indicates if the mouse was pressed in the last frame. */

    /**
     * @brief Retrieves the tile data at the specified position.
     * @param x The X position of the tile.
     * @param y The Y position of the tile.
     * @return The tile data.
     */
    pluginsData::Tile getTile(int x, int y);

    /**
     * @brief Detects collision with a player.
     */
    void detectCollisionPlayer();

    /**
     * @brief Draws information about a specific player.
     * @param id The ID of the player.
     * @param font Shared pointer to the GUI font.
     */
    void drawPlayerInfo(int id, std::shared_ptr<irr::gui::IGUIFont> font);

 public:
    /**
     * @brief Draws the UI elements for the plugin.
     * @param font Shared pointer to the GUI font.
     * @param driver Pointer to the video driver.
     */
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;

    /**
     * @brief Handles events for the plugin.
     * @param event The event to handle.
     */
    bool onEvent(const irr::SEvent &event, pluginsData &datas) override;
};
