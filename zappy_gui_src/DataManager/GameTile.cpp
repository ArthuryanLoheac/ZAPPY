#include <memory>
#include <vector>
#include <utility>

#include "DataManager/GameTile.hpp"
#include "Graphic/Window/window.hpp"

namespace GUI {
GameTile::GameTile(int xCoord, int yCoord)
: x(xCoord), y(yCoord) {}

GameTile::GameTile(GameTile &&other) noexcept
: x(other.x), y(other.y), tileMesh(std::move(other.tileMesh)),
eggs(std::move(other.eggs)) {}

GameTile &GameTile::operator=(GameTile &&other) noexcept {
    if (this != &other) {
        std::lock_guard<std::mutex> lock(mutexDatas);
        x = other.x;
        y = other.y;
        tileMesh = std::move(other.tileMesh);
        eggs = std::move(other.eggs);
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

const std::vector<GameTile::Egg> &GameTile::getEggs() const {
    return eggs;
}

void GameTile::addEgg(int id, int team) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    Vec3d position = getWorldPos();
    position.Y += 0.2f;
    eggs.emplace_back(id, team, importMesh("DroneEgg", position, Vec3d(0.25f)));
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

// ======= Egg ========= //

GameTile::Egg::Egg(int id, int team,
const std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> &eggMesh)
: id(id), team(team), EggMesh(eggMesh) {
    if (!EggMesh)
        throw GUI::ShaderCompilationException("Error creating egg mesh");
}
}  // namespace GUI
