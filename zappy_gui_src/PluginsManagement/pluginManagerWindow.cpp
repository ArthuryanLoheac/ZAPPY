#include <dlfcn.h>
#include <dirent.h>
#include <irrlicht/irrlicht.h>

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <algorithm>

#include "include/logs.h"
#include "PluginsManagement/pluginsManager.hpp"
#include "DataManager/DataManager.hpp"
#include "Graphic/Window/window.hpp"
#include "Connection/ServerGUI.hpp"
#include "PluginsManagement/PluginsDataManager.hpp"
#include "DataManager/PathManager.hpp"

void pluginsManager::drawImage(const std::string &texture, int x,
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

void pluginsManager::onEventWindow(const irr::SEvent &event)
{
    (void) event;
}

void pluginsManager::drawWindow(std::shared_ptr<irr::gui::IGUIFont> font,
    irr::video::IVideoDriver* driver)
{
    int y = 250;
    int x = driver->getScreenSize().Width / 2 - 150;
    int i = 0;

    drawImage("assets/UI/All.png", x - 30,
        y - 30, 350, 30 * (showedPlugins + 2), driver, 255);
    for (auto &plugin : _plugins) {
        if (plugin && i >= pluginIndex && i < pluginIndex + showedPlugins) {
            font->draw(plugin->getName().c_str(),
                irr::core::rect<irr::s32>(x, y, 0, 30),
                irr::video::SColor(255, 255, 255, 255), false, false);
            y += 30;
        }
        i++;
    }
}

void pluginsManager::updateWindow(pluginsData dataManager) {
    (void) dataManager;
}
