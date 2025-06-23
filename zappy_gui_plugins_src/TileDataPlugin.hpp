#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/include/Aplugin.hpp"

/**
 * @class TileDataPlugin
 * @brief Plugin class for handling tile data visualization and interaction.
 * @implements Aplugin
 */
class TileDataPlugin : public Aplugin {
 private:
    int xTile = -1; /**< X position of the selected tile. */
    int yTile = -1; /**< Y position of the selected tile. */
    std::shared_ptr<irr::gui::IGUIFont> font;
    irr::video::IVideoDriver* driver;

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
     * @brief Detects collision with a tile.
     */
    bool detectCollisionTile();

    int countTileRessource(pluginsData::Tile tile);

    void drawRessources(pluginsData::Tile tile, int &y);
    void drawEggs(pluginsData::Tile tile, int &y);
    void drawPlayers(pluginsData::Tile tile, int &y);

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

    std::string getName() const override {
        return "Tile Data Plugin";
    }

    int getPriority() const override {
        return 0;
    }
};
