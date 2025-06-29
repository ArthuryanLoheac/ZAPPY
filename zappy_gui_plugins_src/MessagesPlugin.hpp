#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/include/Aplugin.hpp"

/**
 * @class MessagesPlugin
 * @brief Plugin class for managing global data visualization and interaction.
 * @implements Aplugin
 */
class MessagesPlugin : public Aplugin {
 private:
    /**
     * @brief Draws a message history on the screen.
     * @param font Shared pointer to the GUI font.
     * @param driver Pointer to the video driver.
     */
    void drawMessageHistory(std::shared_ptr<irr::gui::IGUIFont> font,
         irr::video::IVideoDriver* driver);

 public:
    /**
     * @brief Draws the UI elements for the plugin.
     * @param font Shared pointer to the GUI font.
     * @param driver Pointer to the video driver.
     */
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;

    std::string getName() const override {
        return "Messages Plugin";
    }
};
