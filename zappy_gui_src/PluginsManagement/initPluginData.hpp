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

    class SkyBox {
     public:
        bool isSet = false;
        float speedRotation = 0;
        std::string top;
        std::string bottom;
        std::string left;
        std::string right;
        std::string front;
        std::string back;
        /** @brief Default constructor for SkyBox. */
        SkyBox() : isSet(false), speedRotation(0), top(""), bottom(""), left(""),
                   right(""), front(""), back("") {}
        /**
         * @brief Constructor for SkyBox with parameters.
         * @param topPath Path to the top texture.
         * @param bottomPath Path to the bottom texture.
         * @param leftPath Path to the left texture.
         * @param rightPath Path to the right texture.
         * @param frontPath Path to the front texture.
         * @param backPath Path to the back texture.
         * @throws std::invalid_argument if any path is empty.
         */
        SkyBox(const std::string &topPath, const std::string &bottomPath,
               const std::string &leftPath, const std::string &rightPath,
               const std::string &frontPath, const std::string &backPath,
               float rotationSpeed = 0) :
            isSet(true), speedRotation(rotationSpeed), top(topPath), bottom(bottomPath),
            left(leftPath), right(rightPath), front(frontPath), back(backPath){
            if (topPath.empty() || bottomPath.empty() || leftPath.empty() ||
                rightPath.empty() || frontPath.empty() || backPath.empty())
                throw std::invalid_argument("Skybox paths cannot be empty");
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

    SkyBox skyBox; /**< Skybox textures. */
};
