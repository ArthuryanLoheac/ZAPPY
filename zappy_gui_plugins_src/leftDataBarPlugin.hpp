#pragma once
#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/pluginsInterface.hpp"

class leftDataBarPlugin : public pluginsInterface {
 private:
    pluginsData data; /**< Data manager for the plugin. */
    bool isActive = true;

   void drawOneBackground(const std::string &texture, int x, int y,
      int sizeX, int sizeY, irr::video::IVideoDriver* driver);

 public:
    bool init() override;
    const char* getName() const override;
    const char* getVersion() const override;
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;
    void onEvent(const irr::SEvent &event) override;
    void update(pluginsData &dataManager) override;
};