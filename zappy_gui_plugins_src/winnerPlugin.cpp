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

bool winnerPlugin::init(irr::scene::ISceneManager* smgr,
    irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) {
    (void) device;
    (void) smgr;
    (void) cam;
    printf("============= Initializing Winner Plugin =============\n");
    return true;
}

void winnerPlugin::drawImage(const std::string &texture, int x,
int y, int sizeX, int sizeY, irr::video::IVideoDriver* driver, int alpha) {
    irr::video::ITexture* bg = nullptr;

    // Check if the texture with the given alpha is already cached
    std::string cacheKey = texture + "_" + std::to_string(alpha);
    auto it = cachedTextures.find(cacheKey);
    if (it != cachedTextures.end()) {
        bg = it->second;
    } else {
        irr::video::ITexture* originalTexture =
            driver->getTexture(texture.c_str());
        if (!originalTexture) {
            std::cerr << "Error: Cant load texture: " << texture << std::endl;
            return;
        }

        irr::video::IImage* image = driver->createImage(originalTexture,
            irr::core::position2d<irr::s32>(0, 0), originalTexture->getSize());
        if (image) {
            for (irr::u32 y = 0; y < image->getDimension().Height; ++y) {
                for (irr::u32 x = 0; x < image->getDimension().Width; ++x) {
                    UICol imageColor = image->getPixel(x, y);
                    if (imageColor.getAlpha() == 0)
                        continue;
                    imageColor.setAlpha(alpha);
                    image->setPixel(x, y, imageColor);
                }
            }
            bg = driver->addTexture(cacheKey.c_str(), image);
            cachedTextures[cacheKey] = bg;
            image->drop();
        }
    }

    if (bg) {
        irr::core::rect<irr::s32> sourceRect(0, 0, 1000, 1000);
        irr::core::rect<irr::s32> destRect(x, y, x + sizeX, y + sizeY);
        driver->draw2DImage(bg, destRect, sourceRect, nullptr, 0, true);
    }
}

void winnerPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver *driver) {
    if (((!data.isGameOver || data.winner.empty()) && !forceActive) || !font || !driver)
        return;
    UICol white(255, 255, 255, 255);
    int w = 300;
    int h = 200;

    int height = driver->getScreenSize().Height;
    int width = driver->getScreenSize().Width;
    drawImage("assets/UI/All.png", width / 2 - (w / 2), height / 2 - (h / 2),
        w, h, driver, 125);
    font->draw(("Winner: " + data.winner).c_str(), irr::core::rect<irr::s32>(
        width / 2 - (w / 2) + 10, height / 2 - (h / 2) - 90,
        width / 2 + (w / 2) - 10, height / 2 + (h / 2) - 10),
        white, true, true, nullptr);
    drawImage("assets/UI/trophy.png", width / 2 - 50,
        height / 2 - 25, 100, 100, driver, 255);
}


bool winnerPlugin::onEvent(const irr::SEvent &event, pluginsData &datas) {
    (void) datas;
    if (event.EventType == irr::EET_KEY_INPUT_EVENT) {
        if (event.KeyInput.Key == irr::KEY_KEY_W && event.KeyInput.PressedDown) {
            forceActive = !forceActive;
            return true;
        }
    }
    return false;
}

void winnerPlugin::update(pluginsData _data) {
    data = _data;
}

int winnerPlugin::getPriority() const {
    return 10;
}
