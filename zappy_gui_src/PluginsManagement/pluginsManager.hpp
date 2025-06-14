#pragma once

#include <string>
#include <vector>
#include <irrlicht/irrlicht.h>
#include <memory>

#include "dlLoader/dlLoader.hpp"
#include "pluginsInterface.hpp"
#include "DataManager/GameDataManager.hpp"

class pluginsManager {
 public:
    static pluginsManager &i() {
        static pluginsManager instance;
        return instance;
    }

    void loadPlugins(const std::string &path);
    void loadPlugin(const std::string &path);

    void drawPlugins(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver) const;
    void onEvent(const irr::SEvent &event);
    void update(pluginsData &dataManager);

 private:
    std::vector<std::unique_ptr<pluginsInterface>> _plugins;
};
