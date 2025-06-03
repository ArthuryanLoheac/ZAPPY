#pragma once
#include <mutex>
#include <vector>
#include <stdexcept>
#include <string>
#include <memory>

#include "DataManager/GameTile.hpp"

namespace GUI {
class GameDataManager {
 public:
    class Egg {
     public:
        int id;
        int team;
        std::shared_ptr<Mesh> EggMesh;
        Egg(int id, int team, const std::shared_ptr<Mesh> &eggMesh);
    };

 private:
    int width;
    int height;
    std::vector<GameTile> tiles;
    std::vector<Egg> eggs;
    std::vector<std::string> teams;

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
    GameTile &addTile(int x, int y);
    GameTile &getTile(int x, int y);
    const std::vector<Egg> &getEggs() const;
    void addEgg(int id, int team, int x, int y);
    void removeEgg(int id);

    void addTeam(const std::string &teamName);
    const std::vector<std::string> &getTeams() const;
};
}  // namespace GUI
