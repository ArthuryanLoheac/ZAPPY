#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <vector>
#include <stdexcept>


class initPluginData {
/** @brief Class representing the data structure */
 public:
    class MeshInitPlugin {
     public:
        bool isSet = false;
        std::string name; /**< Name of the mesh. */
        float scale; /**< Scale of the mesh. */

        /** @brief Default constructor for MeshInitPlugin. */
        MeshInitPlugin() : isSet(false), name(""), scale(1.0f) {}
        /**
         * @brief Constructor for MeshInitPlugin with parameters.
         * @param meshName The name of the mesh.
         * @param meshScale The scale of the mesh (default is 1.0f).
         * @throws std::invalid_argument if meshName is empty.
         */
        explicit MeshInitPlugin(const std::string &meshName,
            float meshScale = 1.0f) :
            isSet(true), name(meshName), scale(meshScale) {
            if (meshName.empty())
                throw std::invalid_argument("Mesh name cannot be empty");
        }
    };

    initPluginData() = default;

    MeshInitPlugin MeshBattery; /**< Path to the battery mesh. */
    MeshInitPlugin MeshMat1; /**< Path to the first material mesh. */
    MeshInitPlugin MeshMat2; /**< Path to the second material mesh. */
    MeshInitPlugin MeshMat3; /**< Path to the third material mesh. */
    MeshInitPlugin MeshMat4; /**< Path to the fourth material mesh. */
    MeshInitPlugin MeshMat5; /**< Path to the fifth material mesh. */
    MeshInitPlugin MeshMat6; /**< Path to the sixth material mesh. */
    MeshInitPlugin MeshPlayer; /**< Path to the player mesh. */
    MeshInitPlugin MeshEgg; /**< Path to the egg mesh. */
    MeshInitPlugin MeshTile; /**< Path to the tile mesh. */
};
