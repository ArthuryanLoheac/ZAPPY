#include <memory>

#include "DataManager/Egg.hpp"

namespace GUI {

/**
 * @brief Constructs a new Egg object.
 *
 * @param id Egg ID.
 * @param team Team associated with the egg.
 * @param eggMesh Mesh representing the egg.
 * @throws ShaderCompilationException If the egg mesh creation fails.
 */
Egg::Egg(int id, int team, int x, int y,
const std::shared_ptr<Mesh> &eggMesh)
: id(id), team(team), x(x), y(y), EggMesh(eggMesh) {}
}  // namespace GUI
