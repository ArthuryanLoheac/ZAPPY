#pragma once
#include <irrlicht/irrlicht.h>

#include "PluginsManagement/pluginsManager.hpp"
#include "Window/windowOptionMenu.hpp"

/**
 * @brief Handles user input events for the graphical interface.
 */
class MyEventReceiver : public irr::IEventReceiver {
 public:
    /**
     * @brief Processes input events and updates key and mouse states.
     *
     * @param event The input event.
     * @return bool Always returns false.
     */
    bool OnEvent(const irr::SEvent& event) override {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
            if (event.MouseInput.Event == irr::EMIE_MOUSE_WHEEL) {
                MouseWheelDelta = event.MouseInput.Wheel;
            }
        }
        windowOptionMenu::i().handleEvent(event);
        pluginsManager::i().onEvent(event);
        return false;
    }

    /**
     * @brief Checks if a specific key is currently pressed.
     *
     * @param keyCode The key code to check.
     * @return bool True if the key is pressed, false otherwise.
     */
    virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const {
        return KeyIsDown[keyCode];
    }

    /**
     * @brief Constructs a new MyEventReceiver object.
     */
    MyEventReceiver() {
        for (irr::u32 i=0; i < irr::KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }

    /**
     * @brief Consumes the mouse wheel delta and resets it to zero.
     *
     * @return float The consumed mouse wheel delta.
     */
    float ConsumeWheelDelta() {
        float delta = MouseWheelDelta;
        MouseWheelDelta = 0.0f;
        return delta;
    }

    /**
     * @brief Gets the value of a key press between two keys.
     *
     * @param plus The key for positive input.
     * @param minus The key for negative input.
     * @return float The resulting value between -1 and 1 based on key presses.
     */
    float getValBetween(irr::EKEY_CODE plus, irr::EKEY_CODE minus) {
        float i = 0;
        i += IsKeyDown(plus);
        i -= IsKeyDown(minus);
        return i;
    }

 private:
    bool KeyIsDown[irr::KEY_KEY_CODES_COUNT]; /**< Array of key states. */
    float MouseWheelDelta; /**< Mouse wheel delta value. */
};
