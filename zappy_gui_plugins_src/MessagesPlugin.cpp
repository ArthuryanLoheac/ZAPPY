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

void MessagesPlugin::drawMessageHistory(
std::shared_ptr<irr::gui::IGUIFont> font, irr::video::IVideoDriver* driver) {
    int height = driver->getScreenSize().Height;
    int delta = 30;
    int y = height - 30 - (data.messages.size() * delta);
    int x = 5;
    UICol color = UICol(255, 255, 255, 255);

    for (const auto &message : data.messages) {
        if (message.content.empty())
            continue;
        std::string text = std::to_string(message.playerId) +
            ": " + message.content;
        if (text.size() > 50)
            text = text.substr(0, 50) + "...";
        drawImage("assets/UI/BottomBig.png", 0, y, 550, delta, driver);
        font->draw(text.c_str(), irr::core::rect<irr::s32>(
            x, y + 5, 650, y + delta), color);
        y += delta;
    }
}

void MessagesPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
    irr::video::IVideoDriver *driver) {
    if (!driver || !font)
        return;
    drawMessageHistory(font, driver);
}
