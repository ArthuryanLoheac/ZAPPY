#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include "zappy_gui_src/PluginsManagement/pluginA.hpp"

/**
 * @class winnerPlugin
 * @brief Plugin class for managing global data visualization and interaction.
 * @implements pluginsInterface
 */
class winnerPlugin : public pluginA {
 public:
    /**
     * @brief Draws the UI elements for the plugin.
     * @param font Shared pointer to the GUI font.
     * @param driver Pointer to the video driver.
     */
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;
};
