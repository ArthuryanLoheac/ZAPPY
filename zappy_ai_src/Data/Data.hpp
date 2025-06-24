#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>

namespace AI {

class Data {
 public:
    Data();
    ~Data();

    static Data &i() {
        static Data instance;
        return instance;
    }

    // basics
    bool isRunning;
    bool isDead;
    bool isInIncantation;
    int mapX;
    int mapY;
    int level;

    // Inventory
    typedef enum Material_e {
        None = 0,
        Food,
        Linemate,
        Deraumere,
        Sibur,
        Mendiane,
        Phiras,
        Thystame
    } Material_t;
    static Material_t parseMaterial(const std::string &matStr);
    std::unordered_map<Material_t, int> inventory;

    // representation of the vision given by the LOOK command, first vector for
    // depth, second for the direction and third is the objects and the number
    // of this object
    std::vector<std::vector<std::unordered_map<std::string, int>>> vision;
    // This method returns the objects at the given coordinates in the vision,
    // where x is the depth and y is the direction (for the direction, it is
    // a negative number for the left, 0 for the middle and a positive for the
    // right).
    std::unordered_map<std::string, int> &mapAt(int x, int y);

    std::queue<std::pair<std::string, int>> messageQueue;

    std::string magicKey;

 private:
};

}  // namespace AI
