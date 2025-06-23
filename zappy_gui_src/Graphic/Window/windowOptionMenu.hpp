#pragma once
#include <irrlicht/irrlicht.h>

#include <vector>
#include <mutex>
#include <memory>
#include <unordered_map>
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

    windowOptionMenu();

    std::unordered_map<std::string, irr::video::ITexture *> cachedTextures;
        /**< Cache for textures to avoid reloading. */
    bool opened = false; /**< Flag to indicate if the option menu is open */

    bool isButtonClicked(Button &button, const irr::SEvent &event);

    void handleEvent(const irr::SEvent &event);

    void drawButton(Button &exitButton, irr::video::IVideoDriver *driver,
        std::shared_ptr<irr::gui::IGUIFont> font, int yOffset, std::string txt,
        int xOffset);

    void draw(irr::video::IVideoDriver *driver,
        std::shared_ptr<irr::gui::IGUIFont> font);

    void drawImage(const std::string &texture, int x,
    int y, int sizeX, int sizeY, irr::video::IVideoDriver* driver, int alpha);
};
