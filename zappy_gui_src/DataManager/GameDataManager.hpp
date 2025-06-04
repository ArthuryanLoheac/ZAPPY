#pragma once
#include <mutex>
#include <vector>
#include <stdexcept>
#include <string>
#include <memory>

#include "DataManager/GameTile.hpp"
#include "DataManager/Player.hpp"
#include "DataManager/Egg.hpp"

namespace GUI {
class GameDataManager {
 private:
    int width;
    int height;
    std::vector<GameTile> tiles;
    std::vector<Egg> eggs;
    std::vector<std::string> teams;
    std::vector<Player> players;

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

    void addPlayer(int id, int x, int y,
        Player::Orientation o, int level, const std::string &teamName);
    Player &getPlayer(int id);
    const std::vector<Player> &getPlayers() const;
    void removePlayer(int id);

    void addTeam(const std::string &teamName);
    const std::vector<std::string> &getTeams() const;
};
}  // namespace GUI
