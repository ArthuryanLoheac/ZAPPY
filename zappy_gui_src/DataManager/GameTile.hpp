#pragma once
#include <string>
#include <vector>
#include <irrlicht/irrlicht.h>
#include <memory>
#include <mutex>
#include "tools.hpp"

namespace GUI {
class GameTile {
 public:
    class Egg {
     public:
        int id;
        int team;
        std::shared_ptr<Mesh> EggMesh;
        Egg(int id, int team,
            const std::shared_ptr<Mesh> &eggMesh);
    };

 private:
    int x;
    int y;
    std::shared_ptr<Mesh> tileMesh;
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

    std::shared_ptr<Mesh> getTileMesh() const;
    void setTileMesh(const std::shared_ptr<Mesh> &mesh);

    const std::vector<Egg> &getEggs() const;
    void addEgg(int id, int team);
};
}  // namespace GUI
