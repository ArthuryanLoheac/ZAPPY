#pragma once
#include <vector>
#include <irrlicht/irrlicht.h>
#include "Exceptions/GraphicalExceptions.hpp"
#include "DataManager/GameDataManager.hpp"
#include <memory>

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
};

