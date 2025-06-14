#include "globalDataPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<globalDataPlugin>();
    }
}

bool globalDataPlugin::init() {
    printf("============= Initializing Test Plugin =============\n");
    return true;
}

const char* globalDataPlugin::getName() const {
    return "Test Plugin";
}

const char* globalDataPlugin::getVersion() const {
    return "1.0.0";
}

void globalDataPlugin::drawOneBackground(const std::string &texture, int x, int y,
int sizeX, int sizeY, irr::video::IVideoDriver* driver) {
    irr::video::ITexture* bg = driver->getTexture(texture.c_str());
    irr::core::rect<irr::s32> sourceRect(0, 0, 1000, 1000);

    irr::core::rect<irr::s32>destRect(x, y, x + sizeX, y + sizeY);
    if (!bg) {
        std::cerr << "Error: Texture not found: " << texture << std::endl;
        return;
    }
    driver->draw2DImage(bg, destRect, sourceRect, 0, nullptr, true);
}

void globalDataPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver* driver) {
    int x = 30;
    int y = 30;
    int spaceBetween = 30;
    UICol white(255, 255, 255, 255);

    if (!font || !isActive)
        return;
    drawOneBackground("assets/UI/BottomRight.png", 0, 0, 150, 400, driver);
    // FPS
    font->draw(("FPS : " + std::to_string(driver->getFPS())).c_str(),
        UIRect(x, y, 300, 50), white);
    // Frequency
    y += spaceBetween;
    font->draw(("Freq : " + std::to_string(data.frequency)).c_str(),
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

void globalDataPlugin::onEvent(const irr::SEvent &event) {
    (void) event;
}

void globalDataPlugin::update(pluginsData &_data) {
    data = _data;
}
