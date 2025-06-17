#include "zappy_gui_plugins_src/frequencyPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <unordered_map>
#include "frequencyPlugin.hpp"

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<frequencyPlugin>();
    }
}

std::unordered_map<std::string, irr::video::ITexture*> cachedTextures;

bool frequencyPlugin::init(irr::scene::ISceneManager* smgr,
    irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) {
    (void) device;
    (void) smgr;
    (void) cam;
    printf("============= Initializing Frequency Plugin =============\n");
    return true;
}

void frequencyPlugin::drawImage(const std::string &texture, int x,
int y, int sizeX, int sizeY, irr::video::IVideoDriver* driver, int alpha) {
    irr::video::ITexture* bg = nullptr;

    // Check if the texture with the given alpha is already cached
    std::string cacheKey = texture + "_" + std::to_string(alpha);
    auto it = cachedTextures.find(cacheKey);
    if (it != cachedTextures.end()) {
        bg = it->second;
    } else {
        // Load and modify the texture if not cached
        irr::video::ITexture* originalTexture = driver->getTexture(texture.c_str());
        if (!originalTexture) {
            std::cerr << "Error: Could not load texture: " << texture << std::endl;
            return;
        }

        irr::video::IImage* image = driver->createImage(originalTexture,
            irr::core::position2d<irr::s32>(0, 0), originalTexture->getSize());
        if (image) {
            for (irr::u32 y = 0; y < image->getDimension().Height; ++y) {
                for (irr::u32 x = 0; x < image->getDimension().Width; ++x) {
                    UICol imageColor = image->getPixel(x, y);
                    imageColor.setAlpha(alpha);
                    image->setPixel(x, y, imageColor);
                }
            }
            bg = driver->addTexture(cacheKey.c_str(), image);
            cachedTextures[cacheKey] = bg; // Cache the modified texture
            image->drop();
        }
    }

    if (bg) {
        irr::core::rect<irr::s32> sourceRect(0, 0, 1000, 1000);
        irr::core::rect<irr::s32> destRect(x, y, x + sizeX, y + sizeY);
        driver->draw2DImage(bg, destRect, sourceRect, nullptr, 0, true);
    }
}

void frequencyPlugin::drawButton(const std::string &texture, int x, int y,
irr::video::IVideoDriver *driver, stateButton buttonState,
const std::string &text, std::shared_ptr<irr::gui::IGUIFont> font) {
    int alpha = 200;
    if (buttonState == DISABLED)
        alpha = 100;
    else if (buttonState == HOVER)
        alpha = 150;
    else if (buttonState == CLICKED)
        alpha = 175;
    UICol white(alpha, 255, 255, 255);

    drawImage(texture, x, y, 30, 30, driver, alpha);
    font->draw(text.c_str(), UIRect(x + 10, y + 5, 50, 30), white);
}

void frequencyPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver *driver) {
    int x = 30;
    int y = 630;
    UICol white(255, 255, 255, 255);

    if (!font || !driver)
        return;
    heightSaved = driver->getScreenSize().Height;
    y = heightSaved - 140;
    drawImage("assets/UI/All.png", 0, y, 150, 110, driver, 125);
    y += 20;
    // Frequency
    font->draw(("Freq : " + std::to_string(data.frequency)).c_str(),
        UIRect(x, y, 300, 50), white);
    // Buttons
    y += 40;
    // MINUS
    drawButton("assets/UI/AllRed.png", 30, y, driver, minusButtonState, "-",
        font);
    // PLUS
    drawButton("assets/UI/AllRed.png", 90, y, driver, plusButtonState, "+",
        font);
}

void frequencyPlugin::checkHoverButton(const irr::SEvent &event,
stateButton &buttonState, int x, int y, int width, int height) {
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
        int mouseX = event.MouseInput.X;
        int mouseY = event.MouseInput.Y;

        if (event.MouseInput.Event == irr::EMIE_MOUSE_MOVED) {
            if (mouseX >= x && mouseX <= x + width &&
                mouseY >= y && mouseY <= y + height) {
                buttonState = HOVER;
            } else {
                buttonState = ENABLED;
            }
        } else if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
            if (mouseX >= x && mouseX <= x + width &&
                mouseY >= y && mouseY <= y + height) {
                buttonState = CLICKED;
            }
        } else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
            buttonState = ENABLED;
        }
    }
}

pluginsData &frequencyPlugin::onEvent(const irr::SEvent &event) {
    if (data.frequency <= 1) {
        minusButtonState = DISABLED;
    } else {
        checkHoverButton(event, minusButtonState, 30, heightSaved - 80, 40, 40);
        if (minusButtonState == CLICKED) {
            if (frequency > 1) {
                frequency--;
                data.frequency = frequency;
            }
        }
    }
    if (data.frequency >= 200) {
        plusButtonState = DISABLED;
    } else {
        checkHoverButton(event, plusButtonState, 90, heightSaved - 80, 40, 40);
        if (plusButtonState == CLICKED) {
            if (frequency < 200) {
                frequency++;
                data.frequency = frequency;
            }
        }
    }
    return data;
}

void frequencyPlugin::update(pluginsData _data) {
    data = _data;
    frequency = data.frequency;
}
