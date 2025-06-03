#include <vector>
#include <string>

#include "DataManager/GameDataManager.hpp"
#include "Exceptions/GraphicalExceptions.hpp"

namespace GUI {
int GameDataManager::getWidth() const {
    return width;
}

int GameDataManager::getHeight() const {
    return height;
}

void GameDataManager::setWidth(int w) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    if (w < 0)
        throw std::invalid_argument("Width must be non-negative");
    width = w;
}

void GameDataManager::setHeight(int h) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    if (h < 0)
        throw std::invalid_argument("Height must be non-negative");
    height = h;
}

GameTile &GameDataManager::addTile(int x, int y) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    tiles.push_back(GameTile(x, y));
    return tiles.back();
}

GameTile &GameDataManager::getTile(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::out_of_range("Tile coordinates out of bounds");
    for (auto &tile : tiles) {
        if (tile.getX() == x && tile.getY() == y)
            return tile;
    }
    throw std::runtime_error("Tile not found");
}
void GameDataManager::addTeam(const std::string &teamName) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    teams.push_back(teamName);
}

const std::vector<GameDataManager::Egg> &GameDataManager::getEggs() const {
    return eggs;
}

void GameDataManager::addEgg(int id, int team, int x, int y) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    Vec3d position = getTile(x, y).getWorldPos();
    position.Y += 0.2f;
    eggs.emplace_back(id, team, MeshImporter::i().importMesh("DroneEgg",
        position, Vec3d(0.2f)));
}

const std::vector<std::string> &GameDataManager::getTeams() const {
    return teams;
}

// ======= Egg ========= //

GameDataManager::Egg::Egg(int id, int team,
const std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> &eggMesh)
: id(id), team(team), EggMesh(eggMesh) {
    if (!EggMesh)
        throw GUI::ShaderCompilationException("Error creating egg mesh");
}
}  // namespace GUI
