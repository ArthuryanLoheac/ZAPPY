#pragma once
#include <irrlicht/irrlicht.h>

#include <vector>
#include <mutex>
#include <memory>
#include <map>
#include <string>
#include <iostream>

#include "Exceptions/GraphicalExceptions.hpp"
#include "DataManager/GameDataManager.hpp"
#include "Graphic/Events/MyEventReceiver.hpp"
#include "DataManager/SoundsManager.hpp"

class windowOptionMenu {
 public:
    static windowOptionMenu &i() {
        static windowOptionMenu instance;
        return instance;
    }

    enum ButtonState {
        BUTTON_NORMAL,
        BUTTON_HOVERED,
        BUTTON_CLICKED
    };

    class Button {
     public:
        ButtonState state = BUTTON_NORMAL;
        float x = 0.0f;
        float y = 0.0f;
        int sizeX;
        int sizeY;
    };
    Button exitButton;
    Button plusMusicButton;
    Button minusMusicButton;
    Button plusSoundButton;
    Button minusSoundButton;
    irr::IrrlichtDevice *device;

    windowOptionMenu() {
        exitButton.sizeX = 350;
        exitButton.sizeY = 50;

        plusSoundButton.sizeX = 50;
        plusSoundButton.sizeY = 50;

        minusSoundButton.sizeX = 50;
        minusSoundButton.sizeY = 50;

        plusMusicButton.sizeX = 50;
        plusMusicButton.sizeY = 50;

        minusMusicButton.sizeX = 50;
        minusMusicButton.sizeY = 50;
    }

    std::unordered_map<std::string, irr::video::ITexture *> cachedTextures;
        /**< Cache for textures to avoid reloading. */
    bool opened = false; /**< Flag to indicate if the option menu is open */

    bool isButtonClicked(Button &button, const irr::SEvent &event) {
        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
            if (event.MouseInput.Event == irr::EMIE_MOUSE_MOVED) {
                int mouseX = event.MouseInput.X;
                int mouseY = event.MouseInput.Y;
                if (mouseX >= button.x && mouseX <= button.x + button.sizeX &&
                    mouseY >= button.y && mouseY <= button.y + button.sizeY) {
                    button.state = BUTTON_HOVERED;
                } else {
                    button.state = BUTTON_NORMAL;
                }
            } else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
                if (button.state == BUTTON_HOVERED) {
                    return true;
                }
            }
        }
        return false;
    }

    void handleEvent(const irr::SEvent &event) {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
            event.KeyInput.Key == irr::KEY_ESCAPE &&
            event.KeyInput.PressedDown)
            opened = !opened;
        if (!opened)
            return;
        if (isButtonClicked(exitButton, event))
            device->closeDevice();
        if (isButtonClicked(minusMusicButton, event))
            GUI::SoundsManager::i().AddVolumeMusic(-5);
        if (isButtonClicked(plusMusicButton, event))
            GUI::SoundsManager::i().AddVolumeMusic(5);
        if (isButtonClicked(minusSoundButton, event))
            GUI::SoundsManager::i().AddVolumeSound(-5);
        if (isButtonClicked(plusSoundButton, event))
            GUI::SoundsManager::i().AddVolumeSound(5);
    }

    void drawButton(Button &exitButton, irr::video::IVideoDriver *driver,
        std::shared_ptr<irr::gui::IGUIFont> font, int yOffset, std::string txt,
        int xOffset) {
        int width = driver->getScreenSize().Width;
        int height = driver->getScreenSize().Height;

        // button exit
        int alpha = exitButton.state == BUTTON_HOVERED ? 200 : 255;
        exitButton.x = width / 2 - 200 + 25 + xOffset;
        exitButton.y = height / 2 - 150 + 100 + yOffset + 10;
        drawImage("assets/UI/AllRed.png",
            exitButton.x, exitButton.y, exitButton.sizeX, exitButton.sizeY, driver, alpha);
        // button exit text
        font->draw(
            txt.c_str(), irr::core::rect<irr::s32>(exitButton.x, exitButton.y,
                exitButton.x + exitButton.sizeX, exitButton.y + exitButton.sizeY),
                irr::video::SColor(255, 255, 255, 255), true, true);
    }

    void draw(irr::video::IVideoDriver *driver,
        std::shared_ptr<irr::gui::IGUIFont> font) {
        if (!opened)
            return;
        int width = driver->getScreenSize().Width;
        int height = driver->getScreenSize().Height;

        drawImage("assets/UI/All.png",
            width / 2 - 200, height / 2 - 150, 400, 300, driver, 255);
        // EXIT
        drawButton(exitButton, driver, font, 120, "Exit", 0);
        // MUSIC
        drawButton(plusMusicButton, driver, font, 30, "+", 300);
        drawButton(minusMusicButton, driver, font, 30, "-", 0);
        font->draw(("Music Volume: " + std::to_string(
            GUI::SoundsManager::i().volumeMusic)).c_str(),
            irr::core::rect<irr::s32>(width / 2 - 200, height / 2 - 150 + 30,
                width / 2 + 200, height / 2 - 150 + 300),
            irr::video::SColor(255, 255, 255, 255), true, true);
        // SOUND
        drawButton(plusSoundButton, driver, font, -70, "+", 300);
        drawButton(minusSoundButton, driver, font, -70, "-", 0);
        font->draw(("Sound Volume: " + std::to_string(
            GUI::SoundsManager::i().volumeSound)).c_str(),
            irr::core::rect<irr::s32>(width / 2 - 200, height / 2 - 150 + 50,
                width / 2 + 200, height / 2 - 150 + 80),
            irr::video::SColor(255, 255, 255, 255), true, true);
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
                std::cerr << "ERR Cant load texture: " << texture << std::endl;
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
