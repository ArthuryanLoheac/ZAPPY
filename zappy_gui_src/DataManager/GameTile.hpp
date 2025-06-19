#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <vector>
#include <memory>
#include <mutex>

#include "tools/MeshImporter.hpp"

namespace GUI {

/**
 * @brief Represents a tile in the game world, including its position and resources.
 */
class GameTile {
 private:
    int x; /**< X-coordinate of the tile. */
    int y; /**< Y-coordinate of the tile. */
    std::shared_ptr<Mesh> tileMesh; /**< Mesh representing the tile. */

    int food; /**< Amount of food on the tile. */
    int r1; /**< Amount of linemate on the tile. */
    int r2; /**< Amount of deraumere on the tile. */
    int r3; /**< Amount of sibur on the tile. */
    int r4; /**< Amount of mendiane on the tile. */
    int r5; /**< Amount of phiras on the tile. */
    int r6; /**< Amount of thystame on the tile. */

    std::vector<std::shared_ptr<Mesh>> meshesFood; /**< Meshes for food. */
    std::vector<std::shared_ptr<Mesh>> meshesR1; /**< Meshes for linemate. */
    std::vector<std::shared_ptr<Mesh>> meshesR2; /**< Meshes for deraumere. */
    std::vector<std::shared_ptr<Mesh>> meshesR3; /**< Meshes for sibur. */
    std::vector<std::shared_ptr<Mesh>> meshesR4; /**< Meshes for mendiane. */
    std::vector<std::shared_ptr<Mesh>> meshesR5; /**< Meshes for phiras. */
    std::vector<std::shared_ptr<Mesh>> meshesR6; /**< Meshes for thystame. */

 public:
    std::mutex mutexDatas; /**< Mutex for thread-safe access. */

    /**
     * @brief Constructs a new GameTile object.
     *
     * @param xCoord X-coordinate of the tile.
     * @param yCoord Y-coordinate of the tile.
     */
    GameTile(int xCoord, int yCoord);

    /**
     * @brief Deleted copy constructor.
     */
    GameTile(const GameTile &) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    GameTile &operator=(const GameTile &) = delete;

    /**
     * @brief Move constructor.
     */
    GameTile(GameTile &&other) noexcept;

    /**
     * @brief Move assignment operator.
     */
    GameTile &operator=(GameTile &&other) noexcept;

    /**
     * @brief Gets the X-coordinate of the tile.
     *
     * @return int X-coordinate of the tile.
     */
    int getX() const;

    /**
     * @brief Gets the Y-coordinate of the tile.
     *
     * @return int Y-coordinate of the tile.
     */
    int getY() const;

    /**
     * @brief Sets the X-coordinate of the tile.
     *
     * @param xCoord New X-coordinate of the tile.
     */
    void setX(int xCoord);

    /**
     * @brief Sets the Y-coordinate of the tile.
     *
     * @param yCoord New Y-coordinate of the tile.
     */
    void setY(int yCoord);

    /**
     * @brief Gets the mesh representing the tile.
     *
     * @return std::shared_ptr<Mesh> Mesh of the tile.
     */
    std::shared_ptr<Mesh> getTileMesh() const;

    /**
     * @brief Sets the mesh representing the tile.
     *
     * @param mesh New mesh for the tile.
     */
    void setTileMesh(const std::shared_ptr<Mesh> &mesh);

    /**
     * @brief Gets the world position of the tile.
     *
     * @return Vec3d World position of the tile.
     */
    Vec3d getWorldPos(Vec3d def = Vec3d(0, 0, 0)) const;

    /**
     * @brief Sets the resources on the tile.
     *
     * @param food Amount of food.
     * @param r1 Amount of resource 1 (linemate).
     * @param r2 Amount of resource 2 (deraumere).
     * @param r3 Amount of resource 3 (sibur).
     * @param r4 Amount of resource 4 (mendiane).
     * @param r5 Amount of resource 5 (phiras).
     * @param r6 Amount of resource 6 (thystame).
     */
    void setRessources(int food, int r1, int r2, int r3, int r4,
                     int r5, int r6);

    /**
     * @brief Gets the amount of a specific resource on the tile.
     *
     * @param id ID of the resource (1-6).
     * @return int Amount of the resource.
     */
    int getRessource(int id) const;

    /**
     * @brief Updates the meshes representing the resources on the tile.
     */
    void updateMeshesRessources();

    /**
     * @brief Updates meshes from a vector of meshes existing in the game tile.
     *
     * @param meshName Name of the mesh.
     * @param count Number of meshes to update.
     * @param meshes Vector of meshes to update.
     * @param scale Scale of the mesh.
     * @param offsetX X-axis offset for the mesh.
     * @param offsetY Y-axis offset for the mesh.
     * @param offsetZ Z-axis offset for the mesh.
     */
    void updateMesh(std::string meshName, int count,
        std::vector<std::shared_ptr<Mesh>> &meshes,
        float scale, float offsetX = 0, float offsetY = 0, float offsetZ = 0);
};

}  // namespace GUI
