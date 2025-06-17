#pragma once
#include <string>
#include <memory>
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

    void drawButton(const std::string &texture, int x, int y,
        irr::video::IVideoDriver* driver,
        stateButton buttonState, const std::string &text,
        std::shared_ptr<irr::gui::IGUIFont> font);

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
    pluginsData &onEvent(const irr::SEvent &event) override;

    /**
     * @brief Updates the plugin with the latest data.
     * @param dataManager Reference to the data manager.
     */
    void update(pluginsData dataManager) override;
};
