#pragma once

#include <memory>

#include "DataManager/GameTile.hpp"
#include "Exceptions/GraphicalExceptions.hpp"

namespace GUI {
class Egg {
 public:
    int id;
    int team;
    std::shared_ptr<Mesh> EggMesh;
    Egg(int id, int team, const std::shared_ptr<Mesh> &eggMesh);
};
}  // namespace GUI
