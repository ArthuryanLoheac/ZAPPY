#pragma once
#include <irrlicht/irrlicht.h>

#include <vector>
#include <memory>

#include "Exceptions/GraphicalExceptions.hpp"
#include "DataManager/GameDataManager.hpp"

namespace GUI {
class Window {
 public:
    Window();
    irr::IrrlichtDevice *device;
    irr::video::IVideoDriver* driver;
    irr::scene::ISceneManager* smgr;
    irr::gui::IGUIEnvironment* guienv;

    std::vector<irr::scene::ISceneNode*> cubes;

    void update();
    void setupWorld();

    static Window &i() {
        static Window instance;
        return instance;
    }
};
}  // namespace GUI
