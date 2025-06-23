#include "zappy_gui_plugins_src/frequencyPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <unordered_map>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<frequencyPlugin>();
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
    int y = 630;
    UICol white(255, 255, 255, 255);
    if (!font || !driver)
        return;

    int x = driver->getScreenSize().Width - 150;
    widthSaved = x + 20;
    heightSaved = driver->getScreenSize().Height;
    y = heightSaved - 140;
    drawImage("assets/UI/All.png", x + 5, y, 150, 110, driver);
    y += 20;
    x += 20;
    // Frequency
    font->draw(("Freq : " + std::to_string(data.frequency)).c_str(),
        UIRect(x + 10, y, 300, 50), white);
    // Buttons
    y += 40;
    drawButton("assets/UI/AllRed.png", x + 20, y, driver, minusButtonState, "-",
        font);
    drawButton("assets/UI/AllRed.png", x + 80, y, driver, plusButtonState, "+",
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

bool frequencyPlugin::onEvent(const irr::SEvent &event, pluginsData &datas) {
    if (data.frequency <= 1) {
        minusButtonState = DISABLED;
    } else {
        checkHoverButton(event, minusButtonState, widthSaved + 20,
            heightSaved - 80, 40, 40);
        if (minusButtonState == CLICKED) {
            if (frequency > 1) {
                frequency--;
                data.frequency = frequency;
                datas.frequency = frequency;
            }
            return true;
        }
    }
    if (data.frequency >= 200) {
        plusButtonState = DISABLED;
    } else {
        checkHoverButton(event, plusButtonState, widthSaved + 80,
            heightSaved - 80, 40, 40);
        if (plusButtonState == CLICKED) {
            if (frequency < 200) {
                frequency++;
                data.frequency = frequency;
                datas.frequency = frequency;
            }
            return true;
        }
    }
    return false;
}

void frequencyPlugin::update(pluginsData _data, float deltaTime) {
    data = _data;
    frequency = data.frequency;
    (void)deltaTime;
}

int frequencyPlugin::getPriority() const {
    return 10;
}
