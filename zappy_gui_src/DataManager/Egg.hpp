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
    int x; /**< X-coordinate of the egg. */
    int y; /**< Y-coordinate of the egg. */
    std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> EggMesh;
        /**< Mesh representing the egg. */

    /**
     * @brief Constructs a new Egg object.
     *
     * @param id Egg ID.
     * @param team Team associated with the egg.
     * @param x X-coordinate of the egg.
     * @param y Y-coordinate of the egg.
     * @param mesh Mesh representing the egg.
     */
    Egg(int id, int team, int x, int y,
        const std::shared_ptr<irr::scene::IAnimatedMeshSceneNode>& mesh);
};

}  // namespace GUI
