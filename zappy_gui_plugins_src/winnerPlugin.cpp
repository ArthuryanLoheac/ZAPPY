#include "zappy_gui_plugins_src/winnerPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <unordered_map>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<winnerPlugin>();
    }
}

void winnerPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver *driver) {
    if (!data.isGameOver || data.winner.empty() || !font || !driver)
        return;
    UICol white(255, 255, 255, 255);
    int w = 300;
    int h = 200;

    int height = driver->getScreenSize().Height;
    int width = driver->getScreenSize().Width;
    drawImage("assets/UI/All.png", width / 2 - (w / 2), height / 2 - (h / 2),
        w, h, driver);
    font->draw(("Winner: " + data.winner).c_str(), irr::core::rect<irr::s32>(
        width / 2 - (w / 2) + 10, height / 2 - (h / 2) - 90,
        width / 2 + (w / 2) - 10, height / 2 + (h / 2) - 10),
        white, true, true, nullptr);
    drawImage("assets/UI/trophy.png", width / 2 - 50,
        height / 2 - 25, 100, 100, driver, 255);
}
