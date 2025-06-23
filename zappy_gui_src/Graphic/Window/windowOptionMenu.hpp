#pragma once
#include <irrlicht/irrlicht.h>

#include <vector>
#include <mutex>
#include <memory>
#include <string>
#include <iostream>

#include "Exceptions/GraphicalExceptions.hpp"
#include "DataManager/GameDataManager.hpp"
#include "Graphic/Events/MyEventReceiver.hpp"

class windowOptionMenu {
 public:
    static windowOptionMenu &i() {
        static windowOptionMenu instance;
        return instance;
    }

    std::unordered_map<std::string, irr::video::ITexture *> cachedTextures;
    /**< Cache for textures to avoid reloading. */
    bool opened = false; /**< Flag to indicate if the option menu is open */

    void handleEvent(const irr::SEvent &event) {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
            event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown) {
            opened = !opened;
        }
    }

    void draw(irr::video::IVideoDriver *driver,
        std::shared_ptr<irr::gui::IGUIFont> font) {
        if (!opened)
            return;
        int width = driver->getScreenSize().Width;
        int height = driver->getScreenSize().Height;

        (void) font;
        drawImage("assets/UI/All.png",
            width / 2 - 200, height / 2 - 150, 400, 300, driver, 255);
    }

    void drawImage(const std::string &texture, int x,
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
                        imageColor.setAlpha(imageColor.getAlpha() *
                            (alpha / 255.0f));
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
};