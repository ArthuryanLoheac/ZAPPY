#pragma once

#include <string>
#include <mutex>
#include <vector>
#include <memory>
#include <utility>

#include "tools/MeshImporter.hpp"

namespace GUI {

/**
 * @brief Represents a player in the game, including position, orientation, level, and resources.
 */
class Player {
 public:
    bool firstSet = true;

    /**
     * @brief Enum for player states.
     *
     * This enum defines the different states a player can be in during the game.
     */
    enum PlayerState {
        MOVING,
        START_ELEVATION,
        IDLE_ELEVATION,
        END_ELEVATION,
    };

    /**
     * @brief Enum for player orientation.
     */
    enum Orientation {
        WEST,
        SOUTH,
        EAST,
        NORTH,
    };

    /**
     * @brief Constructs a new Player object.
     *
     * @param id Player ID.
     * @param x Initial X-coordinate.
     * @param y Initial Y-coordinate.
     * @param o Initial orientation.
     * @param l Initial level.
     * @param team Team name.
     * @param pMesh Mesh associated with the player.
     */
    Player(int id, int x, int y, Orientation o, int l, const std::string &team,
           const std::shared_ptr<Mesh> &pMesh);

    /**
     * @brief Move constructor.
     */
    Player(Player &&other) noexcept;

    /**
     * @brief Move assignment operator.
     */
    Player &operator=(Player &&other) noexcept;

    /**
     * @brief Deleted copy constructor.
     */
    Player(const Player &) = delete;

    /**
     * @brief Deleted copy assignment operator.
     */
    Player &operator=(const Player &) = delete;

    /**
     * @brief Destructor.
     */
    ~Player() {}

    /**
     * @brief Initializes the player with the given team and level.
     *
     * @param team Team name.
     * @param level Player level.
     */
    void Init(std::string team, int level);

    /**
     * @brief Sets the player ID.
     *
     * @param newId New player ID.
     */
    void setId(int newId);

    /**
     * @brief Gets the player ID.
     *
     * @return Player ID.
     */
    int getId() const;

    /**
     * @brief Sets the X-coordinate of the player.
     *
     * @param newX New X-coordinate.
     */
    void setX(int newX);

    /**
     * @brief Gets the X-coordinate of the player.
     *
     * @return X-coordinate.
     */
    int getX() const;

    /**
     * @brief Sets the Y-coordinate of the player.
     *
     * @param newY New Y-coordinate.
     */
    void setY(int newY);

    /**
     * @brief Gets the Y-coordinate of the player.
     *
     * @return Y-coordinate.
     */
    int getY() const;

    /**
     * @brief Sets the orientation of the player.
     *
     * @param newO New orientation.
     */
    void setOrientation(Orientation newO);

    /**
     * @brief Gets the orientation of the player.
     *
     * @return Player orientation.
     */
    Orientation getOrientation() const;

    /**
     * @brief Sets the level of the player.
     *
     * @param newLevel New level.
     */
    void setLevel(int newLevel);

    /**
     * @brief Gets the level of the player.
     *
     * @return Player level.
     */
    int getLevel() const;

    /**
     * @brief Sets the team name of the player.
     *
     * @param newTeamName New team name.
     */
    void setTeamName(const std::string &newTeamName);

    /**
     * @brief Gets the team name of the player.
     *
     * @return Team name.
     */
    const std::string &getTeamName() const;

    /**
     * @brief Sets the position and orientation of the player.
     *
     * @param newX New X-coordinate.
     * @param newY New Y-coordinate.
     * @param newO New orientation.
     */
    void setPosition(int newX, int newY, Orientation newO);
    /**
     * @brief Sets the position and orientation of the player with TP option
     *
     * @param newX New X-coordinate.
     * @param newY New Y-coordinate.
     * @param newO New orientation.
     * @param TP Teleportation flag (true for TP, false for normal movement).
     */
    void setPosition(int newX, int newY, Orientation newO, bool TP);

    /**
     * @brief Sets the position of the player.
     *
     * @param newX New X-coordinate.
     * @param newY New Y-coordinate.
     */
    void setPosition(int newX, int newY);

    /**
     * @brief Sets the mesh associated with the player.
     *
     * @param mesh New mesh.
     */
    void setMesh(const std::shared_ptr<Mesh> &mesh);

    /**
     * @brief Gets the mesh associated with the player.
     *
     * @return Mesh.
     */
    std::shared_ptr<Mesh> getMesh() const;

    /**
     * @brief Sets the value of a resource.
     *
     * @param id Resource ID.
     * @param value Resource value.
     */
    void setRessource(int id, int value);

    /**
     * @brief Gets the value of a resource.
     *
     * @param id Resource ID.
     * @return Resource value.
     */
    int getRessource(int id) const;

    /**
     * @brief Destroys the player object.
     */
    void destroy();

    /**
     * @brief Checks the angle difference between two vectors.
     *
     * @param a First vector.
     * @param b Second vector.
     * @return True if the angle difference is within acceptable limits, false otherwise.
     */
    bool checkAngleDiff(Vec3d a, Vec3d b);

    /**
     * @brief Updates the player state based on the elapsed time.
     *
     * This function updates the player's position, rotation, and state based on the
     * elapsed time since the last update.
     *
     * @param deltaTime Time elapsed since the last update.
     */
    void Update(float deltaTime);

    /**
     * @brief Sets the elevation state of the player.
     *
     * This function sets whether the player is starting or ending an elevation.
     *
     * @param isStart True if starting elevation, false if ending elevation.
     */
    void setElevation(bool isStart);

    /**
     * @brief Updates the player state when moving.
     *
     * This function updates the player's position and rotation while moving.
     *
     * @param deltaTime Time elapsed since the last update.
     */
    void updateStartElevation(float deltaTime);

    /**
     * @brief Updates the player state during elevation.
     *
     * This function updates the player's rotation while in the elevation state.
     *
     * @param deltaTime Time elapsed since the last update.
     */
    void updateElevation(float deltaTime);

    /**
     * @brief Updates the player state when ending elevation.
     *
     * This function updates the player's rotation and position when ending elevation.
     *
     * @param deltaTime Time elapsed since the last update.
     */
    void updateEndElevation(float deltaTime);

    /**
     * @brief Updates the player state when moving.
     *
     * This function updates the player's position and rotation while moving.
     *
     * @param deltaTime Time elapsed since the last update.
     */
    void UpdateMoving(float deltaTime);

    /**
     * @brief Updates the rotation of the player based on the elapsed time.
     *
     * @param deltaTime Time elapsed since the last update.
     */
    void updateRotation(float deltaTime);

    /**
     * @brief Updates the position of the player based on the elapsed time.
     *
     * @param deltaTime Time elapsed since the last update.
     */
    void updatePosition(float deltaTime);

    /**
     * @brief Updates the idle state of the player based on the elapsed time.
     *
     * @param deltaTime Time elapsed since the last update.
     */
    void updtaeIdle(float deltaTime);

    void initMeshRings();

 private:
    int id; /**< Player ID. */
    int x; /**< X-coordinate of the player. */
    int y; /**< Y-coordinate of the player. */
    Vec3d posTarget; /**< Target position of the player. */
    Vec3d rotationTarget; /**< Target rotation of the player. */
    float speedMove = 1; /**< Movement speed of the player. */
    float baseSpeedMove = 1.f / 7.f; /**< Base movement speed of the player. */
    float idlePosY = 0.5f; /**< Idle position Y-coordinate. */
    float timeTT = 0; /**< Time tracking variable. */

    Orientation o; /**< Orientation of the player. */
    int level; /**< Level of the player. */
    std::string teamName; /**< Team name of the player. */
    std::mutex mutexDatas; /**< Mutex for thread-safe data access. */
    std::shared_ptr<Mesh> PlayerMesh; /**< Mesh associated with the player. */
    std::vector<std::shared_ptr<Mesh>> PlayerMeshesCylinder;
        /**< Rings meshes associated with the player. */
    std::vector<Vec3d> PlayerMeshesCylinderRotation;
        /**< Rotation vectors for Rings meshes. */

    PlayerState state = MOVING; /**< Current state of the player. */
    bool isElevation = false; /**< Flag indicating if the player is elevating*/
    Vec3d deltaRotPlayer; /**< Rotation delta for the player. */
    float speed = 1; /**< Speed of the player during elevation. */

    std::vector<int> ressources = {0, 0, 0, 0, 0, 0, 0};
        /**< Resources held by the player. */

    const int maxLevel = 8; /**< Maximum level of the player. */
};

}  // namespace GUI
