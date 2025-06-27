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
    isInIncantation = false;
    level = 1;
    vision = {};

    inventory = {
        {Material_t::Food, 10},
        {Material_t::Linemate, 0},
        {Material_t::Deraumere, 0},
        {Material_t::Sibur, 0},
        {Material_t::Mendiane, 0},
        {Material_t::Phiras, 0},
        {Material_t::Thystame, 0}
    };
}

Data::~Data() {
}

const std::unordered_map<std::string, AI::Data::Material_t>
AI::Data::stringToMaterial = {
    {"food", Material_t::Food},
    {"linemate", Material_t::Linemate},
    {"deraumere", Material_t::Deraumere},
    {"sibur", Material_t::Sibur},
    {"mendiane", Material_t::Mendiane},
    {"phiras", Material_t::Phiras},
    {"thystame", Material_t::Thystame}
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

}  // namespace AI
