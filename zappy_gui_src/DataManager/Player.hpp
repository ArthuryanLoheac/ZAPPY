#pragma once

#include <string>
#include <mutex>

namespace GUI {
class Player {
 public:
    enum Orientation {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };

    Player();
    Player(int id, int x, int y, Orientation o, int level, const std::string &teamName)
        : id(id), x(x), y(y), o(o), level(level), teamName(teamName) {}
    void setId(int newId);
    int getId() const;
    void setX(int newX);
    int getX() const;
    void setY(int newY);
    int getY() const;
    void setOrientation(Orientation newO);
    Orientation getOrientation() const;
    void setLevel(int newLevel);
    int getLevel() const;
    void setTeamName(const std::string &newTeamName);
    const std::string &getTeamName() const;
    void setPosition(int newX, int newY);

 private:
    int id;
    int x;
    int y;
    Orientation o;
    int level;
    std::string teamName;
    std::mutex mutexDatas;
};
}  // namespace GUI
