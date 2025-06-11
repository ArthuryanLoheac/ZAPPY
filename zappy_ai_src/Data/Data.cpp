#include <string>
#include <unordered_map>
#include <stdexcept>

#include "Data/Data.hpp"

namespace AI {

Data::Data() {
    mapX = 0;
    mapY = 0;
    isDead = false;
    isRunning = false;
    vision = {};
}

Data::~Data() {
}

// ————————————————————————— Definition of methods ————————————————————————

std::unordered_map<std::string, int> &Data::mapAt(int x, int y) {
    if (x < 0 || x >= static_cast<int>(vision.size()))
        throw std::out_of_range("Depth out of range");
    y += vision[x].size() / 2;
    if (y < 0 || y >= static_cast<int>(vision[x].size()))
        throw std::out_of_range("Direction out of range");
    return vision[x][y];
}

}  // namespace AI
