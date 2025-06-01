#pragma once
#include <irrlicht/irrlicht.h>
#include <string>
#include <memory>
#include <map>

#define Mesh irr::scene::IAnimatedMeshSceneNode
#define Vec3d irr::core::vector3df

class MeshImporter {
 public:
    static MeshImporter &i() {
        static MeshImporter instance;
        return instance;
    }

    std::map<std::string, irr::scene::IAnimatedMesh *> meshes;
    std::map<std::string, irr::video::ITexture *> textures;

    std::shared_ptr<Mesh> importMesh(std::string meshName,
            const Vec3d &position = Vec3d(0, 0, 0),
            const Vec3d &scale = Vec3d(1, 1, 1),
            const Vec3d &rotation = Vec3d(0, 0, 0));
    irr::scene::IAnimatedMesh *getMesh(std::string meshName);
    irr::video::ITexture *getTexture(std::string textureName);
};

