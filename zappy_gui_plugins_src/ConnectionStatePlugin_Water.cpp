#include "zappy_gui_plugins_src/ConnectionStatePlugin_Water.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<ConnectionStatePlugin_Water>();
    }
}

void ConnectionStatePlugin_Water::update(pluginsData data, float deltaTime) {
    this->data = data;
    durationRemaining -= deltaTime;
    if (durationRemaining <= 0) {
        durationRemaining = durationFrame;
        frame += 1;
        if (frame > 10)
            frame = 1;
    }
}

void ConnectionStatePlugin_Water::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
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
        drawImage("assets/UI/WaterLoading/" + std::to_string(frame) + ".png", width / 2 + 20, 10, 30, 30,
            driver);
}
