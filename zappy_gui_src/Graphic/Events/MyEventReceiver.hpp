#pragma once

#include <irrlicht/irrlicht.h>

class MyEventReceiver : public irr::IEventReceiver {
 public:
    bool OnEvent(const irr::SEvent& event) override {
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
            if (event.MouseInput.Event == irr::EMIE_MOUSE_WHEEL) {
                MouseWheelDelta = event.MouseInput.Wheel;
            }
        }
        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
            if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
                pressed = true;
            } else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
                pressed = false;
            }
        }
        return false;
    }

    virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const {
        return KeyIsDown[keyCode];
    }

    void updateLastPressed() {
        Lastpressed = pressed;
    }

    virtual bool IsMouseDown() const {
        return pressed && !Lastpressed;
    }

    MyEventReceiver() {
        for (irr::u32 i=0; i < irr::KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }

    float ConsumeWheelDelta() {
        float delta = MouseWheelDelta;
        MouseWheelDelta = 0.0f;
        return delta;
    }

    float getValBetween(irr::EKEY_CODE plus, irr::EKEY_CODE minus) {
        float i = 0;
        i += IsKeyDown(plus);
        i -= IsKeyDown(minus);
        return i;
    }

 private:
    bool KeyIsDown[irr::KEY_KEY_CODES_COUNT];
    bool Lastpressed = false;
    bool pressed = false;
    float MouseWheelDelta;
};
