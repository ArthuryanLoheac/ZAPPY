#include <memory>
#include <vector>
#include <utility>
#include <string>

#include "DataManager/GameTile.hpp"
#include "Graphic/Window/window.hpp"
#include "tools/MeshImporter.hpp"
#include "DataManager/PathManager.hpp"

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

void GameTile::clear(irr::scene::ISceneManager *smgr) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    if (!smgr)
        return;
    tileMesh->remove();
    tileMesh = nullptr;
    for (auto &mesh : meshesFood)
        mesh->remove();
    meshesFood.clear();
    for (auto &mesh : meshesR1)
        mesh->remove();
    meshesR1.clear();
    for (auto &mesh : meshesR2)
        mesh->remove();
    meshesR2.clear();
    for (auto &mesh : meshesR3)
        mesh->remove();
    meshesR3.clear();
    for (auto &mesh : meshesR4)
        mesh->remove();
    meshesR4.clear();
    for (auto &mesh : meshesR5)
        mesh->remove();
    meshesR5.clear();
    for (auto &mesh : meshesR6)
        mesh->remove();
    meshesR6.clear();
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

Vec3d GameTile::getWorldPos(Vec3d def) const {
    if (!tileMesh)
        return def;
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
}

int GameTile::getRessource(int id) const {
    switch (id) {
        case 0: return food;
        case 1: return r1;
        case 2: return r2;
        case 3: return r3;
        case 4: return r4;
        case 5: return r5;
        case 6: return r6;
        default:
            throw std::out_of_range("Invalid resource ID");
    }
}

void GameTile::updateMeshesRessources() {
    updateMesh(PathManager::i().getPath("Food"),
        food, meshesFood, 0.05f, 0.35f, 0.3f, 0.35f);
    updateMesh(PathManager::i().getPath("Mat1"),
        r1, meshesR1, 0.1f, -0.35f, 0.15f, 0.35f);
    updateMesh(PathManager::i().getPath("Mat2"),
        r2, meshesR2, 0.1f, 0.35f, 0.15f, -0.35f);
    updateMesh(PathManager::i().getPath("Mat3"),
        r3, meshesR3, 0.1f, -0.35f, 0.15f, -0.35f);
    updateMesh(PathManager::i().getPath("Mat4"),
        r4, meshesR4, 0.1f, 0.15f, 0.15f, 0.35f);
    updateMesh(PathManager::i().getPath("Mat5"),
        r5, meshesR5, 0.1f, -0.15f, 0.15f, 0.35f);
    updateMesh(PathManager::i().getPath("Mat6"),
        r6, meshesR6, 0.1f, 0.15f, 0.15f, -0.35f);
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
        meshActual->setScale(Vec3d(scale) *
            PathManager::i().getScale(meshName));
        meshActual->setVisible(true);
    }
}

}  // namespace GUI
