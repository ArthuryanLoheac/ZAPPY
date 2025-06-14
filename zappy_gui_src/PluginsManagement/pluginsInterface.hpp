#pragma once
#include <irrlicht/irrlicht.h>

#include <memory>
#include <string>
#include <vector>
#define UIRect irr::core::rect<irr::s32>
#define UICol irr::video::SColor

#include "pluginsData.hpp"

class pluginsInterface {
 public:
    virtual ~pluginsInterface() = default;

    /**
    * @brief Initialize the plugin.
    * @return true if initialization was successful, false otherwise.
    */
    virtual bool init(irr::scene::ISceneManager* smgr,
        irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) = 0;

    virtual void update(pluginsData &dataManager) = 0;

    virtual void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver) = 0;

    virtual void onEvent(const irr::SEvent &event) = 0;
};
