#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/include/Aplugin.hpp"

/**
 * @class graphRessourcesPlugin
 * @brief Plugin class for managing global data visualization and interaction.
 * @implements Aplugin
 */
class graphRessourcesPlugin : public Aplugin {
 private:
    std::vector<int> computeRessources();
    std::vector<std::vector<int>> lstRessources;
    float restingTime = 0;

    void addList(std::vector<int> lst);

    void drawImageReplace(const std::string &texture, int x,
        int y, int sizeX, int sizeY, irr::video::IVideoDriver* driver,
        irr::video::SColor replace);

    float getMaxRessource();

 public:
    /**
     * @brief Draws the UI elements for the plugin.
     * @param font Shared pointer to the GUI font.
     * @param driver Pointer to the video driver.
     */
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;

    void update(pluginsData dataManager, float deltaTime) override;

    std::string getName() const override {
        return "Graph Ressources Plugin";
    }
};
