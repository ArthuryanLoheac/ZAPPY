#pragma once
#include <irrlicht/irrlicht.h>

#include <vector>
#include <memory>

#include "Exceptions/GraphicalExceptions.hpp"
#include "DataManager/GameDataManager.hpp"
#include "MyEventReceiver.hpp"

namespace GUI {
class Window {
 public:
    Window();
    irr::IrrlichtDevice *device;
    irr::video::IVideoDriver* driver;
    irr::scene::ISceneManager* smgr;
    irr::gui::IGUIEnvironment* guienv;
    irr::scene::ICameraSceneNode *cam;
    MyEventReceiver receiver;
    irr::u32 then;
    irr::f32 frameDeltaTime;

    std::vector<irr::scene::ISceneNode*> cubes;

    float rotationSpeedCamera = 100.f;
    float zoomSpeedCamera = 20.f;
    float angleXCamera = 0;
    float distanceFromCenter = 10.f;

    void update();
    void setupWorld();
    void handleEvent();
    void updateDeltaTime();

    void moveCamera(float x, float zoom);

    static Window &i() {
        static Window instance;
        return instance;
    }
};
}  // namespace GUI
