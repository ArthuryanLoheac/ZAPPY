#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/include/Aplugin.hpp"

/**
 * @class ConnectionStatePlugin_Water
 * @brief Plugin class for managing global data visualization and interaction.
 * @implements Aplugin
 */
class ConnectionStatePlugin_Water : public Aplugin {
 private:
    int frame = 1;
    float durationRemaining = 0.1f;
    float durationFrame = 0.1f;

 public:
    /**
     * @brief Draws the UI elements for the plugin.
     * @param font Shared pointer to the GUI font.
     * @param driver Pointer to the video driver.
     */
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;

    void update(pluginsData data, float dt) override;

    std::string getName() const override {
        return "Connection State Water";
    }
};
