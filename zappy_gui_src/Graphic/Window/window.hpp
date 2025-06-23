#pragma once
#include <irrlicht/irrlicht.h>

#include <vector>
#include <mutex>
#include <memory>
#include <string>
#include <iostream>

#include "Exceptions/GraphicalExceptions.hpp"
#include "DataManager/GameDataManager.hpp"
#include "Graphic/Events/MyEventReceiver.hpp"

#define UIRect irr::core::rect<irr::s32>
#define UICol irr::video::SColor
namespace GUI {

/**
 * @brief Manages the graphical window and rendering for the application.
 */
class Window {
 public:
    std::mutex mutexDatas; /**< Mutex for thread-safe access. */
    /**
     * @brief Constructs a new Window object.
     */
    Window();
    /**
     * @brief Sets up the skybox for the scene.
     */
    void SetupSkybox();
    irr::IrrlichtDevice *device; /**<device for rendering the scene*/
    irr::video::IVideoDriver* driver; /**<video driver for rendering*/
    irr::scene::ISceneManager* smgr; /**<scene manager for managing the scene*/
    irr::gui::IGUIEnvironment* guienv;
        /**<GUI environment for managingGUI elements*/
    irr::scene::ICameraSceneNode *cam; /**<camera for viewing the scene*/
    MyEventReceiver receiver; /**<event receiver for handling user input*/
    irr::u32 then; /**<timestamp for frame timing*/
    irr::f32 frameDeltaTime; /**<time delta for frame updates*/
    std::shared_ptr<irr::gui::IGUIFont> font;
        /**<font for rendering textin the GUI*/
    std::vector<irr::scene::ISceneNode*> cubes;
        /**<list of cubes in the scene*/

    std::vector<int> missingPlayersInit;
        /**<list of missing players initialisation*/
    std::vector<int> missingEggsInit;
        /**<list of missing eggs initialisation*/

    // lights
    irr::scene::ILightSceneNode *light = nullptr;
        /**<light source for the scene*/

    std::shared_ptr<irr::scene::ISceneNode> Skybox;
        /**<skybox node forthe scene*/
    Vec3d rotationSkybox; /**<rotation of the skybox*/
    float speedRotationSkybox = 1; /**<speed of the skybox rotation*/

    float rotationSpeedCamera = 100.f; /**<speed of camera rotation*/
    float zoomSpeedCamera = 20.f; /**<speed of camera zooming*/
    float moveSpeedCamera = 5.f; /**<speed of camera movement*/

    float angleXCamera = 0; /**<angle of the camera in the X direction*/
    float distanceFromCenter = 10.f;
        /**<distance of the camera from the center of the scene*/

    bool worldSetuped = false; /**<flag to indicate if the world is set up*/
    bool needUpdateRessources = false; /**<flag if resources need update*/
    bool needUpdatePlayers = false; /**<flag if players need update*/
    bool needUpdateEggs = false; /**<flag to indicate if eggs need updating*/

    /**
     * @brief Updates the window and handles rendering.
     */
    void update();

    /**
     * @brief Clears all meshes in the scene.
     *
     * This function clears all meshes, including player meshes, egg meshes,
     * and tile meshes, and resets the world setup state.
     */
    void clearMeshes();

    /**
     * @brief check if the mesh need to be initialized
     * @return true if the mesh need to be initialized
     * @return false if the mesh is already initialized
     */
    void updateMesh();

    /**
     * @brief Initializes the mesh for players.
     */
    void initMeshPlayers();

    /**
     * @brief Initializes the mesh for eggs.
     */
    void initMeshEggs();

    /**
     * @brief Initializes the mesh resources.
     *
     * This function sets up the necessary mesh resources for the game.
     */
    void initMeshRessources();

    /**
     * @brief Removes a player from the initialization list.
     *
     * @param id The ID of the player to remove.
     */
    void removePlayerInitLst(int id);

    /**
     * @brief Adds a player to the initialization list.
     *
     * @param id The ID of the player to add.
     */
    void addPlayerInitLst(int id);

    /**
     * @brief Removes a Egg from the initialization list.
     *
     * @param id The ID of the Egg to remove.
     */
    void removeEggInitLst(int id);

    /**
     * @brief Adds a Egg to the initialization list.
     *
     * @param id The ID of the Egg to add.
     */
    void addEggInitLst(int id);

    /**
     * @brief Sets up the tile meshes for the game world.
     */
    void worldSetupMesh();

    /**
     * @brief Sets up the game world.
     */
    void setupWorld();

    /**
     * @brief Handles user input events.
     */
    void handleEvent();

    /**
     * @brief Updates the time delta between frames.
     */
    void updateDeltaTime();

    /**
     * @brief Moves the camera based on user input.
     *
     * @param x Horizontal movement.
     * @param zoom Zoom level.
     * @param xMove Horizontal movement of the camera target.
     * @param yMove Vertical movement of the camera target.
     * @param zMove Depth movement of the camera target.
     */
    void moveCamera(float x, float zoom, float xMove, float yMove, float zMove);

    /**
     * @brief Updates the zoom level of the camera.
     *
     * @param zoom Zoom level.
     */
    void updateZoomCamera(float zoom);

    /**
     * @brief Updates the camera's target position.
     *
     * @param xMove Horizontal movement.
     * @param yMove Vertical movement.
     * @param zMove Depth movement.
     * @param radX Rotation in the X direction.
     * @param radZ Rotation in the Z direction.
     */
    void updateMoveOrigin(float xMove, float yMove, float zMove, float radX, float radZ);

    /**
     * @brief Updates the camera's rotation.
     *
     * @param x Rotation amount.
     */
    void updateRotation(float x);

    /**
     * @brief Updates the rotation of the skybox.
     */
    void updateSkyBoxRotation();

    /**
     * @brief Updates the window when it has focus.
     */
    void windowUpdateFocus();

    /**
     * @brief Updates the window when it does not have focus.
     */
    void windowUpdateNoFocus();

    /**
     * @brief Provides a singleton instance of the Window class.
     *
     * @return Window& Reference to the singleton instance.
     */
    static Window &i() {
        static Window instance;
        return instance;
    }

    /**
     * @brief Sets whether the players need to be updated.
     *
     * @param b True if resources need updating, false otherwise.
     */
    void setUpdatePlayer(bool b);

    void setRotationSpeedSkybox(float speed);
};

}  // namespace GUI
