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

    typedef enum class Material_e {
        Food,
        Linemate,
        Deraumere,
        Sibur,
        Mendiane,
        Phiras,
        Thystame
    } Material_t;
    // Inventory
    typedef std::unordered_map<Material_t, int> Inventory_t;
    Inventory_t inventory;
    static const std::unordered_map<std::string, Material_t> stringToMaterial;
    static inline std::string materialToString(Material_t mat) {
        switch (mat) {
            case Material_t::Food:      return "Food";
            case Material_t::Linemate:  return "Linemate";
            case Material_t::Deraumere: return "Deraumere";
            case Material_t::Sibur:     return "Sibur";
            case Material_t::Mendiane:  return "Mendiane";
            case Material_t::Phiras:    return "Phiras";
            case Material_t::Thystame:  return "Thystame";
            default:                    return "Unknown";
        }
    }

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

inline std::string operator+(const std::string &lhs, AI::Data::Material_t mat) {
    return lhs + AI::Data::materialToString(mat);
}
