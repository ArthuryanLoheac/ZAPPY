#pragma once
#include <irrlicht/irrlicht.h>

#include <vector>
#include <memory>

#include "Exceptions/GraphicalExceptions.hpp"
#include "DataManager/GameDataManager.hpp"
#include "Graphic/Events/MyEventReceiver.hpp"

#define UIRect irr::core::rect<irr::s32>
#define UICol irr::video::SColor
namespace GUI {
class Window {
 private:
    void SetupSkybox();
 public:
    Window();
    irr::IrrlichtDevice *device;
    irr::video::IVideoDriver* driver;
    irr::scene::ISceneManager* smgr;
    irr::gui::IGUIEnvironment* guienv;
    irr::scene::ICameraSceneNode *cam;
    MyEventReceiver receiver;
    irr::u32 then;
    irr::f32 frameDeltaTime;
    std::shared_ptr<irr::gui::IGUIFont> font;
    std::vector<irr::scene::ISceneNode*> cubes;

    std::shared_ptr<irr::scene::ISceneNode> Skybox;
    Vec3d rotationSkybox;

    float rotationSpeedCamera = 100.f;
    float zoomSpeedCamera = 20.f;
    float moveSpeedCamera = 5.f;

    int xTile = -1;
    int yTile = -1;
    int idPlayer = -1;

    float angleXCamera = 0;
    float distanceFromCenter = 10.f;

    void update();
    void setupWorld();
    void handleEvent();
    void updateDeltaTime();

    void moveCamera(float x, float zoom, float xMove, float yMove);
    void updateZoomCamera(float zoom);
    void updateMoveOrigin(float xMove, float yMove, float radX, float radZ);
    void updateRotation(float x);
    void updateSkyBoxRotation();

    void handleCLick();
    bool detectCollisionGround();
    bool detectCollisionPlayer();

    void drawOneBackground(int x, int y, int sizeX, int sizeY);
    void drawBackgrounds();
    void drawUI();
    void drawTileInfo(GameTile &tile, int &y);
    void drawPlayerInfo(int id, int &y);

    static Window &i() {
        static Window instance;
        return instance;
    }
};
}  // namespace GUI
