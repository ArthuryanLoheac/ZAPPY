#pragma once
#include <irrlicht/irrlicht.h>

#include <memory>
#include <string>
#include <vector>

#define UIRect irr::core::rect<irr::s32>
#define UICol irr::video::SColor

#include "./pluginsData.hpp"
#include "./initPluginData.hpp"

class pluginsInterface {
/** @brief Interface for plugins in the Zappy GUI application.
 * This interface defines the methods that all plugins must implement
 * to interact with the game data and rendering system.
 */
 public:
    virtual ~pluginsInterface() = default;

    /**
    * @brief Initialize the plugin.
    * @param smgr Pointer to the scene manager used for rendering.
    * @param device Pointer to the Irrlicht device used for rendering.
    * @param cam Pointer to the camera scene node used for rendering.
    * @return True if the plugin was initialized successfully, false otherwise.
    */
    virtual initPluginData init(irr::scene::ISceneManager* smgr,
        irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) = 0;

    /** @brief Update the plugin with the current game data.
     * @param dataManager Reference to the pluginsData object containing all the datas
     */
    virtual void update(pluginsData dataManager) = 0;

    /** @brief Draw the plugin's UI elements.
     * @param font Pointer to the GUI font used for rendering text.
     * @param driver Pointer to the video driver used for rendering graphics.
     */
    virtual void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver) = 0;

    /** @brief Handle events for the plugin.
     * @param event The event to handle.
     */
    virtual bool onEvent(const irr::SEvent &event, pluginsData &datas) = 0;

    /** @brief Get the priority of the plugin.
     * The priority determines the order in which plugins are processed.
     * @return The priority level of the plugin.
     */
    virtual int getPriority() const = 0;

    /** @brief Check if the plugin is currently active.
     * @return True if the plugin is active, false otherwise.
     */
    virtual bool isActive() const = 0;

    /** @brief Set the active state of the plugin.
     * @param active True to activate the plugin, false to deactivate it.
     */
    virtual void setActive(bool active) = 0;
};
