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
    pluginData.MeshPlayer = initPluginData::MeshInitPlugin("Kirby", 0.8f);
    pluginData.MeshEgg = initPluginData::MeshInitPlugin("KirbyEgg", 0.8f);
    pluginData.MeshRing = initPluginData::MeshInitPlugin("Star", 0.8f);
    pluginData.Ambient = initPluginData::SoundInitPlugin(
        "Musics/SC_WorldPyro.mp3");
    pluginData.skyBox = initPluginData::SkyBox(
        "kirbySkybox.jpg", "kirbySkybox.jpg",
        "kirbySkybox.jpg", "kirbySkybox.jpg",
        "kirbySkybox.jpg", "kirbySkybox.jpg", 0);
    // RED
    pluginData.light = initPluginData::LightPlugin(
        irr::video::SColorf(1.5f, 1.5f, 2.f, 1.5f));
    return pluginData;
}
