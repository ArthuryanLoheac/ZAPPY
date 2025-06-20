#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include "zappy_gui_src/PluginsManagement/pluginsInterface.hpp"

/**
 * @class winnerPlugin
 * @brief Plugin class for managing global data visualization and interaction.
 * @implements pluginsInterface
 */
class winnerPlugin : public pluginsInterface {
 private:
    pluginsData data; /**< Data manager for the plugin. */

    std::unordered_map<std::string, irr::video::ITexture *> cachedTextures;
        /**< Cache for textures to avoid reloading. */

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
     * @brief Initializes the plugin with the necessary Irrlicht components.
     * @param smgr Pointer to the scene manager.
     * @param device Pointer to the Irrlicht device.
     * @param cam Pointer to the camera scene node.
     * @return True if initialization is successful, false otherwise.
     */
    bool init(irr::scene::ISceneManager* smgr,
    irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) override;

    /**
     * @brief Gets the priority of the plugin.
     * @return The priority level of the plugin.
     */
    int getPriority() const override;

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
