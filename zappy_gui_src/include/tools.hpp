#pragma once
#include <irrlicht/irrlicht.h>
#include <string>
#include <memory>

#define Mesh irr::scene::IAnimatedMeshSceneNode
#define Vec3d irr::core::vector3df

std::shared_ptr<Mesh>
importMesh(irr::scene::ISceneManager *smgr,
        irr::video::IVideoDriver *driver, std::string meshName,
        const Vec3d &position = Vec3d(0, 0, 0),
        const Vec3d &scale = Vec3d(1, 1, 1),
        const Vec3d &rotation = Vec3d(0, 0, 0));

