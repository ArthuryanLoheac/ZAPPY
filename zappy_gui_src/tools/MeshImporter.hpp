#pragma once
#include <irrlicht/irrlicht.h>
#include <string>
#include <memory>
#include <map>
#include <vector>

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
            const std::string &teamName = "",
            const Vec3d &position = Vec3d(0, 0, 0),
            const Vec3d &scale = Vec3d(1, 1, 1),
            const Vec3d &rotation = Vec3d(0, 0, 0));
    irr::scene::IAnimatedMesh *getMesh(std::string meshName);
    irr::video::ITexture *getTexture(std::string textureName);
    void setTexture(std::string meshName, const std::string &teamName,
        std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> node);
    irr::video::SColor getColor(std::string teamName);

    std::map<std::string, irr::video::SColor> colorTeam;
    size_t iColor = 0;
    std::vector<irr::video::SColor> colors = {
        irr::video::SColor(255, 41, 115, 115),  // Green/Blue
        irr::video::SColor(255, 255, 133, 82),  // Orange
        irr::video::SColor(255, 167, 212, 155),  // Grreen
        irr::video::SColor(255, 137, 6, 32),  // Red
        irr::video::SColor(255, 255, 159, 229),  // Pink
        irr::video::SColor(255, 233, 215, 88),  // Yellow
        irr::video::SColor(255, 226, 194, 255)  // Magenta
    };
};

