#pragma once
#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/pluginsInterface.hpp"

/**
 * @class PlayerDataPlugin
 * @brief Plugin class for managing player data visualization and interaction
 * when clicking on one
 * @implements pluginsInterface
 */
class PlayerDataPlugin : public pluginsInterface {
 private:
    pluginsData data; /**< Data manager for the plugin. */
    int idPlayer = -1; /**< ID of the selected player. */

    irr::scene::ISceneManager* smgr;
      /**< Pointer to the Irrlicht scene manager*/
    irr::IrrlichtDevice *device; /**< Pointer to the Irrlicht device. */
    irr::scene::ICameraSceneNode *cam; /**< Pointer to the Irrlicht camera. */
    irr::video::IVideoDriver* driver; /**< Pointer to the video driver. */

    bool pressed = false; /**< Indicates if the mouse is currently pressed. */
    bool isPressedLastFrame = false;
      /**< Indicates if the mouse was pressed in the last frame. */

    std::vector<std::string> lstNames = {"Food", "Linemate", "Deraumere",
        "Sibur", "Mendiane", "Phiras", "Thystame"};
        /**< List of resource names. */

    std::vector<UICol> lstColors = {
        UICol(255, 255, 255, 255), UICol(255, 200, 193, 198),
        UICol(255, 105, 105, 105), UICol(255, 121, 123, 166),
        UICol(255, 94, 84, 33), UICol(255, 144, 81, 82),
        UICol(255, 114, 85, 144)}; /**< List of colors representing resources*/

    /**
     * @brief Draws a background texture at a specified position and size.
     * @param texture The texture file path.
     * @param x The X position.
     * @param y The Y position.
     * @param sizeX The width of the background.
     * @param sizeY The height of the background.
     * @param driver Pointer to the video driver.
     */
    void drawOneBackground(const std::string &texture, int x, int y,
      int sizeX, int sizeY, irr::video::IVideoDriver* driver);

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
     * @brief Initializes the plugin with the necessary Irrlicht components.
     * @param smgr Pointer to the scene manager.
     * @param device Pointer to the Irrlicht device.
     * @param cam Pointer to the camera scene node.
     * @return True if initialization is successful, false otherwise.
     */
    bool init(irr::scene::ISceneManager* smgr,
      irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) override;

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

    /**
     * @brief Updates the plugin with the latest data.
     * @param dataManager Reference to the data manager.
     */
    void update(pluginsData dataManager) override;
};
