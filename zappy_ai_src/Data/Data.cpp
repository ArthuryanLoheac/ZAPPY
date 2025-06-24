#include <string>
#include <unordered_map>
#include <stdexcept>
#include <string_view>

#include "Data/Data.hpp"
#include "Data.hpp"

namespace AI {

Data::Data() {
    mapX = 0;
    mapY = 0;
    isDead = false;
    isRunning = false;
    isInIncantation = false;
    level = 1;
    vision = {};

    inventory = {
        {Food, 10},
        {Linemate, 0},
        {Deraumere, 0},
        {Sibur, 0},
        {Mendiane, 0},
        {Phiras, 0},
        {Thystame, 0}
    };
}

Data::~Data() {
}

const std::unordered_map<std::string, Data::Material_t> StringToMaterial =
{
    {"Food", Data::Food}
};

// ————————————————————————— Definition of methods ————————————————————————

std::unordered_map<std::string, int> &Data::mapAt(int x, int y) {

    if (x < 0 || x >= static_cast<int>(vision.size()))
        throw std::out_of_range("Depth out of range");
    y += vision[x].size() / 2;
    if (y < 0 || y >= static_cast<int>(vision[x].size()))
        throw std::out_of_range("Direction out of range");
    return vision[x][y];
}

Data::Material_t Data::parseMaterial(const std::string &matStr)
{
    if (!StringToMaterial.count(matStr))
        return None;
    else
        return StringToMaterial.at(matStr);
}

}  // namespace AI
