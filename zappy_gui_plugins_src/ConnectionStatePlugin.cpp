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
    UICol color = data.ping < 100 ? UICol(125, 255, 255, 255) :
        data.ping < 200 ? UICol(125, 255, 255, 0) :
        UICol(125, 255, 0, 0);

    font->draw(std::to_string(data.ping).c_str(),
        irr::core::rect<irr::s32>(width / 2 - 20, 10, width / 2 + 20, 50),
        color, false, true);
    if (data.isConnected)
        drawImage("assets/UI/connected.png", width / 2 + 20, 10, 30, 30,
            driver);
    else
        drawImage("assets/UI/disconnected.png", width / 2 + 20, 10, 30, 30,
            driver);
}

bool ConnectionStatePlugin::onEvent(const irr::SEvent &event,
pluginsData &datas) {
    (void) event;
    (void) datas;
    return false;
}

void ConnectionStatePlugin::update(pluginsData _data) {
    data = _data;
}

int ConnectionStatePlugin::getPriority() const {
    return 0;
}
