#pragma once
#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/pluginsInterface.hpp"

class testPlugin : public pluginsInterface {
 private:
    bool isActive = true;

 public:
    bool init() override;
    const char* getName() const override;
    const char* getVersion() const override;
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;
    void onEvent(const irr::SEvent &event) override;
    void update(pluginsData &dataManager) override;
};