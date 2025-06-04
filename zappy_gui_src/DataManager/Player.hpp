#pragma once

#include <string>
#include <mutex>
#include <memory>
#include <utility>

#include "tools/MeshImporter.hpp"

namespace GUI {
class Player {
 public:
    enum Orientation {
        WEST,
        SOUTH,
        EAST,
        NORTH,
    };

    Player(int id, int x, int y, Orientation o, int l, const std::string &team,
           const std::shared_ptr<Mesh> &pMesh)
        : id(id), x(x), y(y), o(o), level(l), teamName(team),
        PlayerMesh(pMesh) {}

    Player(Player &&other) noexcept
        : id(other.id), x(other.x), y(other.y), o(other.o), level(other.level),
          teamName(std::move(other.teamName)),
          PlayerMesh(std::move(other.PlayerMesh)) {}
    Player &operator=(Player &&other) noexcept;
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;

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
    void setPosition(int newX, int newY, Orientation newO);
    void setPosition(int newX, int newY);

    void setMesh(const std::shared_ptr<Mesh> &mesh);
    std::shared_ptr<Mesh> getMesh() const;

    void setRessource(int id, int value);
    int getRessource(int id) const;

 private:
    int id;
    int x;
    int y;
    Orientation o;
    int level;
    std::string teamName;
    std::mutex mutexDatas;
    std::shared_ptr<Mesh> PlayerMesh;

    std::vector<int> ressources = {0, 0, 0, 0, 0, 0, 0};
};
}  // namespace GUI
