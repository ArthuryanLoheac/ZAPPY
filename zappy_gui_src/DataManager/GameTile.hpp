#pragma once
#include <string>
#include <vector>
#include <irrlicht/irrlicht.h>
#include <memory>

namespace GUI {
class GameTile {
 public:
    class Egg {
     public:
        std::string team;
        std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> EggMesh;
    };

    int x;
    int y;
    std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> tileMesh;
    std::vector<Egg> eggs;
    GameTile(int xCoord, int yCoord) : x(xCoord), y(yCoord) {}
};
}  // namespace GUI
