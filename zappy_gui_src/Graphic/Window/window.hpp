#pragma once
#include <irrlicht/irrlicht.h>

#include <vector>
#include <memory>
#include <string>

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
 private:
    /**
     * @brief Sets up the skybox for the scene.
     */
    void SetupSkybox();

 public:
    /**
     * @brief Constructs a new Window object.
     */
    Window();
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
    std::vector<irr::scene::ISceneNode*> cubes; /**<list of cubes in the scene*/

    std::shared_ptr<irr::scene::ISceneNode> Skybox;
        /**<skybox node forthe scene*/
    Vec3d rotationSkybox; /**<rotation of the skybox*/

    float rotationSpeedCamera = 100.f; /**<speed of camera rotation*/
    float zoomSpeedCamera = 20.f; /**<speed of camera zooming*/
    float moveSpeedCamera = 5.f; /**<speed of camera movement*/

    int xTile = -1; /**<x-coordinate of the selected tile*/
    int yTile = -1; /**<y-coordinate of the selected tile*/
    int idPlayer = -1; /**<ID of the selected player*/

    float angleXCamera = 0; /**<angle of the camera in the X direction*/
    float distanceFromCenter = 10.f;
        /**<distance of the camera from the center of the scene*/

    /**
     * @brief Updates the window and handles rendering.
     */
    void update();

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
     */
    void moveCamera(float x, float zoom, float xMove, float yMove);

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
     * @param radX Rotation in the X direction.
     * @param radZ Rotation in the Z direction.
     */
    void updateMoveOrigin(float xMove, float yMove, float radX, float radZ);

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
     * @brief Handles mouse click events.
     */
    void handleCLick();

    /**
     * @brief Detects collisions with the ground when the user clicks.
     *
     * @return bool True if a collision is detected, false otherwise.
     */
    bool detectCollisionGround();

    /**
     * @brief Detects collisions with players when the user clicks.
     *
     * @return bool True if a collision is detected, false otherwise.
     */
    bool detectCollisionPlayer();

    /**
     * @brief Draws a single background UI element.
     *
     * @param texture Path to the texture.
     * @param x X-coordinate.
     * @param y Y-coordinate.
     * @param sizeX Width of the background.
     * @param sizeY Height of the background.
     */
    void drawOneBackground(const std::string &texture, int x, int y,
        int sizeX, int sizeY);

    /**
     * @brief Draws all background UI elements.
     */
    void drawBackgrounds();

    /**
     * @brief Draws the user interface.
     */
    void drawUI();

    /**
     * @brief Draws information about a specific tile in the UI
     *
     * @param tile The tile to display information about.
     * @param y Vertical position for drawing.
     */
    void drawTileInfo(GameTile &tile, int &y);

    /**
     * @brief Draws information about a specific player in the UI.
     *
     * @param id The player's ID.
     * @param y Vertical position for drawing.
     */
    void drawPlayerInfo(int id, int &y);

    /**
     * @brief Provides a singleton instance of the Window class.
     *
     * @return Window& Reference to the singleton instance.
     */
    static Window &i() {
        static Window instance;
        return instance;
    }
};

}  // namespace GUI
