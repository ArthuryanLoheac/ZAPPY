#pragma once
#include <irrlicht/irrlicht.h>
#include "Exceptions/GraphicalExceptions.hpp"

class Window {
 public:
    Window();
    irr::IrrlichtDevice *device;
    irr::video::IVideoDriver* driver;
    irr::scene::ISceneManager* smgr;
    irr::gui::IGUIEnvironment* guienv;

    void update();
};

