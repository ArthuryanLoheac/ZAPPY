#include <iostream>
#include <string>
#include "include/pluginA.hpp"

initPluginData pluginA::init(irr::scene::ISceneManager* smgr,
    irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) {
    this->smgr = smgr;
    this->device = device;
    this->cam = cam;
    return initPluginData();
}

void pluginA::update(pluginsData dataManager) {
    data = dataManager;
}

bool pluginA::onEvent(const irr::SEvent &event, pluginsData &datas) {
    (void)event;
    (void)datas;
    return false;
}

int pluginA::getPriority() const {
    return 0;
}

void pluginA::drawImage(const std::string &texture, int x,
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
