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
    MyEventReceiver receiver;

    std::vector<irr::scene::ISceneNode*> cubes;

    void update();
    void setupWorld();
    void handleEvent();

    static Window &i() {
        static Window instance;
        return instance;
    }
};
}  // namespace GUI
