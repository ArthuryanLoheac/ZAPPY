#include <vector>
#include <string>
#include <memory>

#include "DataManager/GameDataManager.hpp"
#include "Exceptions/GraphicalExceptions.hpp"
#include "Exceptions/DataManagerExceptions.hpp"
#include "Window/window.hpp"
#include "GameDataManager.hpp"

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
    throw std::runtime_error("Tile " + std::to_string(x) +
        "/" + std::to_string(y) + " not found");
}
void GameDataManager::addTeam(const std::string &teamName) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    teams.push_back(teamName);
}

const std::vector<Egg> &GameDataManager::getEggs() const {
    return eggs;
}

void GameDataManager::addEgg(int id, int team, int x, int y) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    Vec3d position = getTile(x, y).getWorldPos();
    position.Y += 0.2f;
    eggs.emplace_back(id, team, MeshImporter::i().importMesh("DroneEgg",
        "", position, Vec3d(0.2f)));
}

void GameDataManager::removeEgg(int id) {
    for (size_t i = 0; i < eggs.size(); i++) {
        if (eggs[i].id == id) {
            int idM = eggs[i].EggMesh->getID();
            if (GUI::Window::i().smgr->getSceneNodeFromId(idM))
                GUI::Window::i().smgr->getSceneNodeFromId(idM)->remove();
            eggs.erase(eggs.begin() + i);
            return;
        }
    }
    throw ParseException("Invalid ID egg");
}

bool GameDataManager::isPlayerAdded() const {
    return playerAdded;
}

void GameDataManager::setPlayerAdded(bool added) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    playerAdded = added;
}

void GameDataManager::addPlayer(int id, int x, int y,
                                Player::Orientation o, int level, const std::string &teamName)
{
    std::lock_guard<std::mutex> lock(mutexDatas);
    Vec3d position = getTile(x, y).getWorldPos();
    position.Y += 0.5f;
    players.emplace_back(id, x, y, o, level, teamName,
        MeshImporter::i().importMesh("Drone", teamName, position, Vec3d(0.2f),
        Vec3d(0, o * 90, 0)));
    playerAdded = true;
}

Player &GameDataManager::getPlayer(int id) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    for (auto &player : players) {
        if (player.getId() == id)
            return player;
    }
    throw ParseException("Player with this ID not found");
}

std::vector<Player> &GameDataManager::getPlayers() {
    return players;
}

void GameDataManager::removePlayer(int id) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].getId() == id) {
            players[i].destroy();
            players.erase(players.begin() + i);
            return;
        }
    }
    throw ParseException("Invalid ID player");
}

const std::vector<std::string> &GameDataManager::getTeams() const {
    return teams;
}

void GameDataManager::Update(float deltaTime) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    for (auto &player : players)
        player.Update(deltaTime);
}

void GameDataManager::setEggAdded(bool added) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    eggAdded = added;
}

bool GameDataManager::isEggAdded() const {
    return eggAdded;
}

bool GameDataManager::isPlayerDead() const {
    return playerDead;
}

void GameDataManager::setPlayerDead(bool dead) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    playerDead = dead;
}

bool GameDataManager::isEggDead() const {
    return eggDead;
}

void GameDataManager::setEggDead(bool dead) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    eggDead = dead;
}

bool GameDataManager::isElevation() const {
    return elevation;
}

void GameDataManager::setElevationSound(bool elev) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    elevation = elev;
}

bool GameDataManager::isCollecting() const {
    return Collecting;
}

void GameDataManager::setCollecting(bool collecting) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    Collecting = collecting;
}

bool GameDataManager::isDropping() const {
    return Dropping;
}

void GameDataManager::setDropping(bool dropping) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    Dropping = dropping;
}

bool GameDataManager::isEggSpawned() const {
    return eggSpawned;
}

void GameDataManager::setEggSpawned(bool spawned) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    eggSpawned = spawned;
}

bool GameDataManager::isPushed() const {
    return Pushed;
}

void GameDataManager::setPushed(bool pushed) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    Pushed = pushed;
}

}  // namespace GUI