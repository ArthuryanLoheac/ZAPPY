#pragma once
#include <irrlicht/irrlicht.h>
#include <string>

std::shared_ptr<irr::scene::IAnimatedMeshSceneNode>
importMesh(irr::scene::ISceneManager *smgr,
        irr::video::IVideoDriver *driver, std::string meshName,
        const irr::core::vector3df &position = irr::core::vector3df(0, 0, 0),
        const irr::core::vector3df &scale = irr::core::vector3df(1, 1, 1),
        const irr::core::vector3df &rotation = irr::core::vector3df(0, 0, 0));
