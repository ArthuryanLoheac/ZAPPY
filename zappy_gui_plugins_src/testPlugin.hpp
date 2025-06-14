#pragma once
#include <string>

#include "zappy_gui_src/PluginsManagement/pluginsInterface.hpp"

class testPlugin : public pluginsInterface {
 public:
    bool init() override;
    const char* getName() const override;
    const char* getVersion() const override;
};