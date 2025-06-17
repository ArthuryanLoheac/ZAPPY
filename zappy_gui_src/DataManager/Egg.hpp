#pragma once

#include <memory>

#include "DataManager/GameTile.hpp"
#include "Exceptions/GraphicalExceptions.hpp"

namespace GUI {

/**
 * @brief Represents an egg in the game, including its ID, team, and associated mesh.
 */
class Egg {
 public:
    int id; /**< Egg ID. */
    int team; /**< Team associated with the egg. */
    std::shared_ptr<Mesh> EggMesh; /**< Mesh representing the egg. */

    /**
     * @brief Constructs a new Egg object.
     *
     * @param id Egg ID.
     * @param team Team associated with the egg.
     * @param eggMesh Mesh representing the egg.
     */
    Egg(int id, int team, const std::shared_ptr<Mesh> &eggMesh);
};

}  // namespace GUI
