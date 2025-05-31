#include "DataManager/GameDataManager.hpp"
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
        if (tile.x == x && tile.y == y)
            return tile;
    }
    throw std::runtime_error("Tile not found");
}
} // namespace GUI
