#include "zappy_gui_plugins_src/ConnectionStatePlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<ConnectionStatePlugin>();
    }
}

bool ConnectionStatePlugin::init(irr::scene::ISceneManager* smgr,
    irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) {
    (void) device;
    (void) smgr;
    (void) cam;
    printf("============= Initializing ConnectionState Plugin =============\n");
    return true;
}

void ConnectionStatePlugin::drawImage(const std::string &texture, int x,
int y, int sizeX, int sizeY, irr::video::IVideoDriver* driver) {
    irr::video::ITexture* bg = driver->getTexture(texture.c_str());
    irr::core::rect<irr::s32> sourceRect(0, 0, 1000, 1000);

    irr::core::rect<irr::s32>destRect(x, y, x + sizeX, y + sizeY);
    if (!bg) {
        std::cerr << "Error: Texture not found: " << texture << std::endl;
        return;
    }
    driver->draw2DImage(bg, destRect, sourceRect, 0, nullptr, true);
}

void ConnectionStatePlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver* driver) {
    if (!driver)
        return;
    int width = driver->getScreenSize().Width;

    (void) font;
    if (data.isConnected)
        drawImage("assets/UI/connected.png", width / 2, 20, 30, 30, driver);
    else
        drawImage("assets/UI/disconnected.png", width / 2, 20, 30, 30, driver);
}

bool ConnectionStatePlugin::onEvent(const irr::SEvent &event,
pluginsData &dataManager) {
    (void) event;
    (void) dataManager;
    return false;
}

void ConnectionStatePlugin::update(pluginsData _data) {
    data = _data;
}

int ConnectionStatePlugin::getPriority() const {
    return 0;
}
