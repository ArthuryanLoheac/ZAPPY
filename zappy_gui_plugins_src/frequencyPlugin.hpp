#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include "zappy_gui_src/PluginsManagement/pluginsInterface.hpp"

/**
 * @class frequencyPlugin
 * @brief Plugin class for managing global data visualization and interaction.
 * @implements pluginsInterface
 */
class frequencyPlugin : public pluginsInterface {
 private:
    enum stateButton {
        DISABLED, /**< Button is disabled. */
        ENABLED, /**< Button is enabled. */
        HOVER, /**< Button is hovered. */
        CLICKED, /**< Button is clicked. */
    };

    pluginsData data; /**< Data manager for the plugin. */
    int frequency = 0; /**< Frequency value to be displayed. */

    stateButton minusButtonState = DISABLED; /**< State of the minus button. */
    stateButton plusButtonState = DISABLED; /**< State of the plus button. */
    int heightSaved = 0; /**< Height of the window. */

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

    /**
     * @brief Draws a button with a texture and text at a specified position.
     * @param texture The texture file path for the button.
     * @param x The X position of the button.
     * @param y The Y position of the button.
     * @param driver Pointer to the video driver.
     * @param buttonState The current state of the button.
     * @param text The text to display on the button.
     * @param font Shared pointer to the GUI font for rendering text.
     */
    void drawButton(const std::string &texture, int x, int y,
        irr::video::IVideoDriver* driver,
        stateButton buttonState, const std::string &text,
        std::shared_ptr<irr::gui::IGUIFont> font);

    /**
     * @brief Checks the state of a button based on mouse events.
     * @param event The mouse event to check.
     * @param buttonState The current state of the button.
     * @param x The X position of the button.
     * @param y The Y position of the button.
     * @param width The width of the button.
     * @param height The height of the button.
     */
    void checkHoverButton(const irr::SEvent &event,
        stateButton &buttonState, int x, int y, int width, int height);

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
