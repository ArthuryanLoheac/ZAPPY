#include "zappy_gui_plugins_src/graphRessourcesPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<graphRessourcesPlugin>();
    }
}

void graphRessourcesPlugin::drawImageReplace(const std::string &texture, int x,
int y, int sizeX, int sizeY, irr::video::IVideoDriver* driver,
irr::video::SColor replace) {
    irr::video::ITexture* bg = nullptr;

    std::string cacheKey = texture  + "_" + std::to_string(replace.getRed())
                                    + "_" + std::to_string(replace.getGreen())
                                    + "_" + std::to_string(replace.getBlue());
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
                    UICol imageColor = replace;
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

float graphRessourcesPlugin::getMaxRessource() {
    int max = 90;

    for (auto ressource : lstRessources) {
        for (size_t i = 0; i < ressource.size(); ++i) {
            if (ressource[i] > max)
                max = ressource[i];
        }
    }
    return static_cast<float>(max);
}

std::vector<int> graphRessourcesPlugin::computeRessources() {
    std::vector<int> ressources(7, 0);
    for (auto tile : data.tiles) {
        if (tile.resources.empty() || tile.resources.size() < 7)
            continue;
        ressources[0] += tile.resources[0];
        ressources[1] += tile.resources[1];
        ressources[2] += tile.resources[2];
        ressources[3] += tile.resources[3];
        ressources[4] += tile.resources[4];
        ressources[5] += tile.resources[5];
        ressources[6] += tile.resources[6];
    }
    return ressources;
}

void graphRessourcesPlugin::addList(std::vector<int> lst) {
    lstRessources.push_back(lst);
    if (lstRessources.size() > 146)
        lstRessources.erase(lstRessources.begin());
}

void graphRessourcesPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
                                   irr::video::IVideoDriver *driver) {
    int y = 300;
    int spaceBetween = 20;
    UICol white(255, 255, 255, 255);
    std::vector<int> ressources = {0, 0, 0, 0, 0, 0, 0};

    if (!font || !driver || lstRessources.size() < 1)
        return;
    ressources = lstRessources.back();
    drawImage("assets/UI/All.png", 0, y, 150, 300, driver);
    y += spaceBetween;
    for (size_t i = 0; i < ressources.size(); ++i) {
        std::string text = lstNames[i] + ": " + std::to_string(ressources[i]);
        font->draw(text.c_str(), irr::core::rect<irr::s32>(10, y, 200, y + 20),
                    lstColors[i], false, true);
        y += spaceBetween;
    }
    y += 100;
    for (size_t i = 0; i < lstRessources.size(); ++i) {
        for (int c = 0; c < 7; c++) {
            drawImageReplace("assets/UI/graphPoint.png", i + 2,
                y - ((lstRessources[i][c] / getMaxRessource()) * 90.f), 1, 1,
                driver, lstColors[c]);
        }
    }
}

void graphRessourcesPlugin::update(pluginsData dataManager, float deltaTime) {
    data = dataManager;
    restingTime -= deltaTime;
    if (restingTime <= 0) {
        std::vector<int> ressources = computeRessources();
        addList(ressources);
        restingTime = 0.2f;
    }
}
