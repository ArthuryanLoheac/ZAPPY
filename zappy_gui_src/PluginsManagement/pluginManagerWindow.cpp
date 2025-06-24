#include <dlfcn.h>
#include <dirent.h>
#include <irrlicht/irrlicht.h>

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <fstream>
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

void pluginsManager::onEventWindow(const irr::SEvent &event) {
    int size = static_cast<int>(_plugins.size());

    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
        if (event.MouseInput.Event == irr::EMIE_MOUSE_WHEEL) {
            if (event.MouseInput.Wheel < 0 &&
                pluginIndex + showedPlugins < size)
                pluginIndex++;
            else if (event.MouseInput.Wheel > 0 && pluginIndex > 0)
                pluginIndex--;
            if (pluginIndex < 0)
                pluginIndex = 0;
            if (pluginIndex + showedPlugins > size)
                pluginIndex = size - showedPlugins;
        }
        if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
            for (const auto &button : buttons) {
                if (event.MouseInput.X >= button.x &&
                    event.MouseInput.X <= button.x + 20 &&
                    event.MouseInput.Y >= button.y &&
                    event.MouseInput.Y <= button.y + 20) {
                    if (_plugins[button.index])
                        _plugins[button.index]->setActive(
                            !_plugins[button.index]->isActive());
                }
            }
        }
    }
}

void pluginsManager::drawWindow(std::shared_ptr<irr::gui::IGUIFont> font,
    irr::video::IVideoDriver* driver) {
    int y = driver->getScreenSize().Height / 2 - (15 * (showedPlugins + 2));
    int x = driver->getScreenSize().Width / 2 - 175;
    int i = 0;

    buttons.clear();
    font->draw("Plugins Manager : ",
        irr::core::rect<irr::s32>(x - 30, y - 50, 0, 30),
        irr::video::SColor(255, 255, 255, 255), false, false);
    drawImage("assets/UI/All.png", x - 30,
        y - 30, 350, 30 * (showedPlugins + 2), driver, 255);
    for (auto &plugin : _plugins) {
        if (plugin && i >= pluginIndex && i < pluginIndex + showedPlugins) {
            font->draw(plugin->getName().c_str(),
                irr::core::rect<irr::s32>(x, y, 0, 30),
                irr::video::SColor(255, 255, 255, 255), false, false);
            y += 30;
            drawButton(font, driver, i, y);
        }
        i++;
    }
}

void pluginsManager::drawButton(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver *driver, int i, int y) {
    if (!_plugins[i])
        return;
    int x = driver->getScreenSize().Width / 2 + 175;
    int alpha = _plugins[i]->isActive() ? 255 : 100;
    std::string buttonText = _plugins[i]->isActive() ? "O" : "X";

    drawImage("assets/UI/AllRed.png", x - 75,
        y - 25, 20, 20, driver, alpha);
    buttons.push_back(ButtonPlugin(x - 75, y - 25, i));
    font->draw(buttonText.c_str(),
        irr::core::rect<irr::s32>(x - 70, y - 25, 0, 20),
        irr::video::SColor(alpha, 255, 255, 255), false, false);
}

void pluginsManager::loadActivePlugins() {
    std::ifstream file(pathSaveFile, std::ios::in);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open plugins save file: %s", pathSaveFile.c_str());
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty())
            continue;
        std::string name = line.substr(0, line.find(':'));
        bool isActive = false;
        try {
            isActive = std::stoi(line.substr(line.find(':') + 1)) == 1;
        } catch (const std::exception &e) {
            LOG_ERROR("Invalid format in plugins save file: %s", e.what());
            continue;
        }

        for (auto &plugin : _plugins) {
            if (plugin && plugin->getName() == name) {
                plugin->setActive(isActive);
                break;
            }
        }
    }
}

void pluginsManager::saveActivePlugins() {
    std::ofstream file(pathSaveFile, std::ios::out);

    if (!file.is_open()) {
        LOG_ERROR("Failed to open plugins save file for writing: %s",
            pathSaveFile.c_str());
        return;
    }
    for (const auto &plugin : _plugins) {
        if (plugin) {
            file << plugin->getName() << ":"
                 << (plugin->isActive() ? "1" : "0") << "\n";
        }
    }
}
