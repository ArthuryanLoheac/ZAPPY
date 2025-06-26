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
    pluginData.MeshTile = initPluginData::MeshInitPlugin("SocleKirby", 1.14f);
    pluginData.MeshMat1 = initPluginData::MeshInitPlugin("Flower1", 1.f);
    pluginData.MeshMat2 = initPluginData::MeshInitPlugin("Flower2", 1.f);
    pluginData.MeshMat3 = initPluginData::MeshInitPlugin("Flower3", 1.f);
    pluginData.MeshMat4 = initPluginData::MeshInitPlugin("Flower4", 1.f);
    pluginData.MeshMat5 = initPluginData::MeshInitPlugin("Flower5", 1.f);
    pluginData.MeshMat6 = initPluginData::MeshInitPlugin("Flower6", 1.f);
    pluginData.MeshBattery = initPluginData::MeshInitPlugin("Coin", 2.f);
    pluginData.Death = initPluginData::SoundInitPlugin(
        "Musics/kirby_-_death_noise-8841fc32-68b4-4605-8593-907d4508a665.mp3");
    pluginData.Spawn = initPluginData::SoundInitPlugin(
        "Musics/hiiii_kirby_sound.mp3");
    pluginData.Elevation = initPluginData::SoundInitPlugin(
        "Musics/kirby_extra_life.mp3");
    pluginData.Push = initPluginData::SoundInitPlugin(
        "Musics/ssb-kirby-captain-falcon-sound.mp3");
    pluginData.SpawnEgg = initPluginData::SoundInitPlugin(
        "Musics/waddle-dee.mp3");
    pluginData.Ambient = initPluginData::SoundInitPlugin(
        "Musics/Kirby dream land theme song.mp3");
    pluginData.skyBox = initPluginData::SkyBox(
        "skybox/SB/SunsetBay_U.png", "skybox/SB/SunsetBay_D.png",
        "skybox/SB/SunsetBay_W.png", "skybox/SB/SunsetBay_E.png",
        "skybox/SB/SunsetBay_N.png", "skybox/SB/SunsetBay_S.png", 0);
    // RED
    pluginData.light = initPluginData::LightPlugin(
        irr::video::SColorf(2.f, 1.f, 1.f, 2.5f));
    return pluginData;
}
