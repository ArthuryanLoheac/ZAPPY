#pragma once
#include <mutex>
#include <stdexcept>
#include "DataManager/GameTile.hpp"

namespace GUI {
class GameDataManager {
 private:
    int width;
    int height;
    std::vector<GameTile> tiles;

 public:
    std::mutex mutexDatas;
    static GameDataManager &i() {
        static GameDataManager i;
        return i;
    }

    GameDataManager() : width(-1), height(-1) {}
    int getWidth() const;
    int getHeight() const;
    void setWidth(int w);
    void setHeight(int h);
    void addTile(int x, int y);
    GameTile &getTile(int x, int y);
};
}  // namespace GUI
