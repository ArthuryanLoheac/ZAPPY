#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <vector>
#include <memory>
#include <mutex>

#include "tools/MeshImporter.hpp"

namespace GUI {
class GameTile {
 private:
    int x;
    int y;
    std::shared_ptr<Mesh> tileMesh;

    int food;
    int r1;
    int r2;
    int r3;
    int r4;
    int r5;
    int r6;

    std::vector<std::shared_ptr<Mesh>> meshesFood;
    std::vector<std::shared_ptr<Mesh>> meshesR1;
    std::vector<std::shared_ptr<Mesh>> meshesR2;
    std::vector<std::shared_ptr<Mesh>> meshesR3;
    std::vector<std::shared_ptr<Mesh>> meshesR4;
    std::vector<std::shared_ptr<Mesh>> meshesR5;
    std::vector<std::shared_ptr<Mesh>> meshesR6;

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
    Vec3d getWorldPos() const;

    void setRessources(int food, int r1, int r2, int r3, int r4,
                     int r5, int r6);
   int getRessource(int id) const;

    void updateMeshesRessources();
    void updateMesh(std::string meshName, int count,
        std::vector<std::shared_ptr<Mesh>> &meshes,
        float scale, float offsetX = 0, float offsetY = 0, float offsetZ = 0);
};
}  // namespace GUI
