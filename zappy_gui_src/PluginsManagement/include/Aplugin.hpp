#pragma once
#include <irrlicht/irrlicht.h>
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "./pluginsInterface.hpp"

class Aplugin : public pluginsInterface {
 protected:
    bool active = true;
    irr::scene::ISceneManager* smgr;
    irr::IrrlichtDevice *device;
    irr::scene::ICameraSceneNode *cam;
    irr::video::IVideoDriver* driver;
    pluginsData data;

    std::unordered_map<std::string, irr::video::ITexture *> cachedTextures;
    /**< Cache for textures to avoid reloading. */

    std::vector<UICol> lstColors = {
        UICol(255, 255, 255, 255), UICol(255, 200, 193, 198),
        UICol(255, 105, 105, 105), UICol(255, 121, 123, 166),
        UICol(255, 94, 84, 33), UICol(255, 144, 81, 82),
        UICol(255, 114, 85, 144)
    }; /**< List of colors representing resources. */

    std::vector<std::string> lstNames = {"Food", "Linemate", "Deraumere",
        "Sibur", "Mendiane", "Phiras", "Thystame"};
        /**< List of resource names. */

    /**
     * @brief Draws a background texture at a specified position and size.
     * @param texture The texture file path.
     * @param x The X position.
     * @param y The Y position.
     * @param sizeX The width of the background.
     * @param sizeY The height of the background.
     * @param driver Pointer to the video driver.
     */
    void drawImage(const std::string &texture, int x, int y,
    int sizeX, int sizeY, irr::video::IVideoDriver* driver, int alpha = 255);

 public:
    /**
    * @brief Initialize the plugin.
    * @param smgr Pointer to the scene manager used for rendering.
    * @param device Pointer to the Irrlicht device used for rendering.
    * @param cam Pointer to the camera scene node used for rendering.
    * @return True if the plugin was initialized successfully, false otherwise.
    */
    initPluginData init(irr::scene::ISceneManager* smgr,
        irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam)
        override;

    /** @brief Update the plugin with the current game data.
     * @param dataManager Reference to the pluginsData object containing all the datas
     */
    void update(pluginsData dataManager) override;

    /** @brief Draw the plugin's UI elements.
     * @param font Pointer to the GUI font used for rendering text.
     * @param driver Pointer to the video driver used for rendering graphics.
     */
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver) = 0;

    /** @brief Handle events for the plugin.
     * @param event The event to handle.
     */
    bool onEvent(const irr::SEvent &event, pluginsData &datas) override;

    /** @brief Get the priority of the plugin.
     * @return The priority level of the plugin.
     */
    int getPriority() const override;

    /** @brief Check if the plugin is currently active.
     * @return True if the plugin is active, false otherwise.
     */
    bool isActive() const override;

    /** @brief Set the active state of the plugin.
     * @param active True to activate the plugin, false to deactivate it.
     */
    void setActive(bool active) override;
};
