#include "zappy_gui_plugins_src/MessagesPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<MessagesPlugin>();
    }
}

bool MessagesPlugin::init(irr::scene::ISceneManager* smgr,
    irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) {
    (void) device;
    (void) smgr;
    (void) cam;
    printf("============= Initializing Messages Plugin =============\n");
    return true;
}

void MessagesPlugin::drawImage(const std::string &texture, int x,
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

void MessagesPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver* driver) {
    if (!driver)
        return;
    int height = driver->getScreenSize().Height;
    int delta = 30;
    int y = height - 30 - (data.messages.size() * delta);
    int x = 5;
    UICol color = UICol(125, 255, 255, 255);

    for (const auto &message : data.messages) {
        if (message.content.empty())
            continue;
        std::string text = std::to_string(message.playerId) +
            ": " + message.content;
        if (text.size() > 50)
            text = text.substr(0, 50) + "...";
        drawImage("assets/UI/BottomBig.png", 0, y, 400, delta, driver);
        font->draw(text.c_str(), irr::core::rect<irr::s32>(
            x, y + 5, 500, y + delta), color);
        y += delta;
    }
}

bool MessagesPlugin::onEvent(const irr::SEvent &event,
pluginsData &datas) {
    (void) event;
    (void) datas;
    return false;
}

void MessagesPlugin::update(pluginsData _data) {
    data = _data;
}

int MessagesPlugin::getPriority() const {
    return 0;
}
