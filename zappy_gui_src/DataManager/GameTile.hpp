#pragma once
#include <string>
#include <vector>
#include <irrlicht/irrlicht.h>
#include <memory>
#include <mutex>

namespace GUI {
class GameTile {
 public:
    class Egg {
     public:
        std::string team;
        std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> EggMesh;
        Egg(const std::string &teamName,
            const std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> &eggMesh);
    };

 private:
    int x;
    int y;
    std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> tileMesh;
    std::vector<Egg> eggs;

 public:
    std::mutex mutexDatas;

    GameTile(int xCoord, int yCoord);

    GameTile(const GameTile &) = delete;
    GameTile &operator=(const GameTile &) = delete;

    GameTile(GameTile &&other) noexcept;
    GameTile &operator=(GameTile &&other) noexcept;

    int getX() const;
    int getY() const;
    void setX(int xCoord);
    void setY(int yCoord);

    std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> getTileMesh() const;
    void setTileMesh(const std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> &mesh);

    const std::vector<Egg> &getEggs() const;
    void addEgg(const std::string &team, const std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> &eggMesh);
};
}  // namespace GUI
