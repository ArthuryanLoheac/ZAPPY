#pragma once
#include <irrlicht/irrlicht.h>
#include <memory>
#include <string>
#include <unordered_map>

#include "./pluginsInterface.hpp"

class pluginA : public pluginsInterface {
 protected:
    irr::scene::ISceneManager* smgr;
    irr::IrrlichtDevice *device;
    irr::scene::ICameraSceneNode *cam;
    irr::video::IVideoDriver* driver;
    pluginsData data;

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
    * @brief Initialize the plugin.
    * @param smgr Pointer to the scene manager used for rendering.
    * @param device Pointer to the Irrlicht device used for rendering.
    * @param cam Pointer to the camera scene node used for rendering.
    * @return True if the plugin was initialized successfully, false otherwise.
    */
    virtual bool init(irr::scene::ISceneManager* smgr,
        irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) override;

    /** @brief Update the plugin with the current game data.
     * @param dataManager Reference to the pluginsData object containing all the datas
     */
    virtual void update(pluginsData dataManager) override;

    /** @brief Draw the plugin's UI elements.
     * @param font Pointer to the GUI font used for rendering text.
     * @param driver Pointer to the video driver used for rendering graphics.
     */
    virtual void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver) = 0;

    /** @brief Handle events for the plugin.
     * @param event The event to handle.
     */
    virtual bool onEvent(const irr::SEvent &event, pluginsData &datas) override;

    virtual int getPriority() const override;
};
