#pragma once
#include <mutex>
#include <stdexcept>

namespace GUI {
class GameDataManager {
 private:
    int width;
    int height;

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
};
}  // namespace GUI