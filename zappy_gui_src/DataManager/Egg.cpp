#include "Egg.hpp"

namespace GUI {
GUI::Egg::Egg(int id, int team, const std::shared_ptr<Mesh> &eggMesh)
: id(id), team(team), EggMesh(eggMesh) {
    if (!EggMesh)
        throw ShaderCompilationException("Error creating egg mesh");
}
} // namespace GUI
