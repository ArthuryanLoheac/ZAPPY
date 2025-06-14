#pragma once
#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/pluginsInterface.hpp"

class testPlugin : public pluginsInterface {
 public:
    bool init() override;
    const char* getName() const override;
    const char* getVersion() const override;
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font) override;
};