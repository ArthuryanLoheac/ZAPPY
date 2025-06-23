#include "zappy_gui_plugins_src/globalDataPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<globalDataPlugin>();
    }
}

void globalDataPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver* driver) {
    int x = 30;
    int y = 30;
    int spaceBetween = 30;
    UICol white(255, 255, 255, 255);

    if (!font)
        return;
    drawImage("assets/UI/BottomRight.png", 0, 0, 150,
        80 + data.teams.size() * 30, driver);
    // FPS
    font->draw(("FPS : " + std::to_string(driver->getFPS())).c_str(),
        UIRect(x, y, 300, 50), white);
    // TEAMS
    y += spaceBetween;
    font->draw("TEAMS : ", UIRect(x, y, 300, 50), white);
    int i = 0;
    for (auto &team : data.teams) {
        y += 20;
        font->draw(("\t" + team).c_str(), UIRect(x, y, 300, 50),
            data.teamColors[i % data.teamColors.size()]);
        i++;
    }
}
