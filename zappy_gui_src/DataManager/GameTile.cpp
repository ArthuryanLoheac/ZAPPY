#include <memory>
#include <vector>
#include <utility>
#include <string>

#include "DataManager/GameTile.hpp"
#include "Graphic/Window/window.hpp"
#include "tools/MeshImporter.hpp"

namespace GUI {
GameTile::GameTile(int xCoord, int yCoord)
: x(xCoord), y(yCoord) {}

GameTile::GameTile(GameTile &&other) noexcept
: x(other.x), y(other.y), tileMesh(std::move(other.tileMesh)) {}

GameTile &GameTile::operator=(GameTile &&other) noexcept {
    if (this != &other) {
        std::lock_guard<std::mutex> lock(mutexDatas);
        x = other.x;
        y = other.y;
        tileMesh = std::move(other.tileMesh);
    }
    return *this;
}

int GameTile::getX() const {
    return x;
}

int GameTile::getY() const {
    return y;
}

void GameTile::setX(int xCoord) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    x = xCoord;
}

void GameTile::setY(int yCoord) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    y = yCoord;
}

std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> GameTile::getTileMesh()
const {
    return tileMesh;
}

void GameTile::setTileMesh(const
std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> &mesh) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    tileMesh = mesh;
}

Vec3d GameTile::getWorldPos() const {
    if (!tileMesh)
        return Vec3d(0, 0, 0);
    return tileMesh->getPosition();
}

void GameTile::setRessources(int food, int r1, int r2, int r3, int r4,
int r5, int r6) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    this->food = food;
    this->r1 = r1;
    this->r2 = r2;
    this->r3 = r3;
    this->r4 = r4;
    this->r5 = r5;
    this->r6 = r6;
    updateMeshesRessources();
}

void GameTile::updateMeshesRessources() {
    updateMesh("Battery", food, meshesFood, 0.05f, 0.35f, 0.3f, 0.35f);
    updateMesh("Mat1", r1, meshesR1, 0.1f, -0.35f, 0.15f, 0.35f);
    updateMesh("Mat2", r2, meshesR2, 0.1f, 0.35f, 0.15f, -0.35f);
    updateMesh("Mat3", r3, meshesR3, 0.1f, -0.35f, 0.15f, -0.35f);
    updateMesh("Mat4", r4, meshesR4, 0.1f, 0.15f, 0.15f, 0.35f);
    updateMesh("Mat5", r5, meshesR5, 0.1f, -0.15f, 0.15f, 0.35f);
    updateMesh("Mat6", r6, meshesR6, 0.1f, 0.15f, 0.15f, -0.35f);
}

void GameTile::updateMesh(std::string meshName, int count,
std::vector<std::shared_ptr<Mesh>> &meshes, float scale,
float offsetX, float offsetY, float offsetZ) {
    // Clear
    for (auto &mesh : meshes)
        mesh->setVisible(false);

    // Create new meshes
    for (int i = 0; i < count; ++i) {
        std::shared_ptr<Mesh> meshActual;
        // create mesh if it doesn't exist
        if (static_cast<size_t>(i) >= meshes.size())
            meshes.push_back(MeshImporter::i().importMesh(meshName));
        meshActual = meshes[i];
        Vec3d position = getWorldPos();
        position.Y += 0.05f + (i * offsetY);
        position.X += offsetX;
        position.Z += offsetZ;
        meshActual->setPosition(position);
        meshActual->setScale(Vec3d(scale));
        meshActual->setVisible(true);
    }
}

}  // namespace GUI
