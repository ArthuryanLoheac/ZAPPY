#include "zappy_gui_plugins_src/frequencyPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<frequencyPlugin>();
    }
}

bool frequencyPlugin::init(irr::scene::ISceneManager* smgr,
    irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) {
    (void) device;
    (void) smgr;
    (void) cam;
    printf("============= Initializing Frequency Plugin =============\n");
    return true;
}

void frequencyPlugin::drawImage(const std::string &texture, int x,
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

void frequencyPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver* driver) {
    int x = 30;
    int y = 630;
    UICol white(255, 255, 255, 255);

    if (!font)
        return;
    drawImage("assets/UI/TopRight.png", 0, 600, 200, 200, driver);
    // Frequency
    font->draw(("Freq : " + std::to_string(data.frequency)).c_str(),
        UIRect(x, y, 300, 50), white);
}

pluginsData &frequencyPlugin::onEvent(const irr::SEvent &event) {
    if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
        if (event.KeyInput.PressedDown == false) {
            if (event.KeyInput.Key == irr::KEY_UP) {
                frequency++;
                data.frequency = frequency;
            } else if (event.KeyInput.Key == irr::KEY_DOWN) {
                if (frequency > 1) {
                    frequency--;
                    data.frequency = frequency;
                }
            }
        }
    }
    return data;
}

void frequencyPlugin::update(pluginsData _data) {
    data = _data;
    frequency = data.frequency;
}
