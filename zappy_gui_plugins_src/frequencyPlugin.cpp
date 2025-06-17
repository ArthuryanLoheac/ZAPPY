#include "zappy_gui_plugins_src/frequencyPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include "frequencyPlugin.hpp"

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
int y, int sizeX, int sizeY, irr::video::IVideoDriver* driver, int alpha) {
    irr::video::ITexture* bg = driver->getTexture(texture.c_str());
    irr::core::rect<irr::s32> sourceRect(0, 0, 1000, 1000);
    irr::core::rect<irr::s32> destRect(x, y, x + sizeX, y + sizeY);

    if (!bg) {
        std::cerr << "Error: Could not load texture: " << texture << std::endl;
        return;
    }

    irr::video::IImage* image = driver->createImage(bg,
        irr::core::position2d<irr::s32>(0, 0), bg->getSize());
    if (image) {
        for (irr::u32 y = 0; y < image->getDimension().Height; ++y) {
            for (irr::u32 x = 0; x < image->getDimension().Width; ++x) {
                UICol imageColor = image->getPixel(x, y);
                imageColor.setAlpha(alpha);
                image->setPixel(x, y, imageColor);
            }
        }
        driver->removeTexture(bg);
        bg = driver->addTexture(texture.c_str(), image);
        image->drop();
    }

    driver->draw2DImage(bg, destRect, sourceRect, nullptr, 0, true);
}

void frequencyPlugin::drawButton(const std::string &texture, int x, int y,
irr::video::IVideoDriver *driver, stateButton buttonState,
const std::string &text, std::shared_ptr<irr::gui::IGUIFont> font) {

    int alpha = 255;
    if (buttonState == DISABLED)
        alpha = 100;
    else if (buttonState == HOVER)
        alpha = 200;
    else if (buttonState == CLICKED)
        alpha = 150;
    UICol white(alpha, 255, 255, 255);

    drawImage(texture, x, y, 30, 30, driver, alpha);
    font->draw(text.c_str(), UIRect(x + 10, y + 5, 50, 30), white);
}

void frequencyPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
                             irr::video::IVideoDriver *driver)
{
    int x = 30;
    int y = 630;
    UICol white(255, 255, 255, 255);

    if (!font || !driver)
        return;
    int height = driver->getScreenSize().Height;
    y = height - 140;
    drawImage("assets/UI/All.png", 0, y, 150, 110, driver, 125);
    y += 20;
    // Frequency
    font->draw(("Freq : " + std::to_string(data.frequency)).c_str(),
        UIRect(x, y, 300, 50), white);
    // Buttons
    y += 40;
    // MINUS
    drawButton("assets/UI/AllRed.png", 30, y, driver, minusButtonState, "-", font);
    // PLUS
    drawButton("assets/UI/AllRed.png", 90, y, driver, plusButtonState, "+", font);
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
        checkHoverButton(event, minusButtonState, 30, 630, 30, 30);
        if (minusButtonState == CLICKED) {
            if (frequency > 1) {
                frequency--;
                data.frequency = frequency;
            }
            minusButtonState = ENABLED;
        }
    }
    if (data.frequency >= 200) {
        plusButtonState = DISABLED;
    } else {
        checkHoverButton(event, plusButtonState, 90, 630, 30, 30);
        if (plusButtonState == CLICKED) {
            if (frequency < 200) {
                frequency++;
                data.frequency = frequency;
            }
            plusButtonState = ENABLED;
        }
    }
    return data;
}

void frequencyPlugin::update(pluginsData _data) {
    data = _data;
    frequency = data.frequency;
}
