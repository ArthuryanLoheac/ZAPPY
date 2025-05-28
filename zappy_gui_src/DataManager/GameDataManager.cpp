#include "DataManager/GameDataManager.hpp"

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
}  // namespace GUI
