#include "zappy_gui_plugins_src/changeBatteryPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <unordered_map>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<changeBatteryPlugin>();
    }
}


void changeBatteryPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver *driver) {
    (void) font;
    (void) driver;
}

initPluginData changeBatteryPlugin::init(irr::scene::ISceneManager* smgr,
    irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) {
    this->smgr = smgr;
    this->device = device;
    this->cam = cam;
    initPluginData pluginData;
    pluginData.MeshBattery = initPluginData::MeshInitPlugin("Battery2", 2.f);
    return pluginData;
}
