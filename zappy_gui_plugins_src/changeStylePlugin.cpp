#include "zappy_gui_plugins_src/changeStylePlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <unordered_map>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<changeStylePlugin>();
    }
}


void changeStylePlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver *driver) {
    (void) font;
    (void) driver;
}

initPluginData changeStylePlugin::init(irr::scene::ISceneManager* smgr,
    irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) {
    this->smgr = smgr;
    this->device = device;
    this->cam = cam;
    initPluginData pluginData;
    pluginData.MeshBattery = initPluginData::MeshInitPlugin("Battery2", 2.f);
    // pluginData.skyBox = initPluginData::SkyBox(
    //     "skybox/skybox2/skybox_py.jpg", "skybox/skybox2/skybox_ny.jpg",
    //     "skybox/skybox2/skybox_nx.jpg", "skybox/skybox2/skybox_px.jpg",
    //     "skybox/skybox2/skybox_pz.jpg", "skybox/skybox2/skybox_nz.jpg", 0);
    return pluginData;
}
