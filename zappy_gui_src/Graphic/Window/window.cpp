#include "Graphic/Window/window.hpp"
#include "Graphic/Events/MyEventReceiver.hpp"
#include "include/tools.hpp"

namespace GUI {
Window::Window() {
    device = irr::createDevice(irr::video::EDT_BURNINGSVIDEO,
        irr::core::dimension2d<irr::u32>(1280, 720), 16, false, true, false,
        &receiver);

    if (!device)
        throw GUI::WindowCreationException("Error creating device");
    device->setWindowCaption(L"Zappy");
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
    // Camera
    cam = smgr->addCameraSceneNode(nullptr,
        irr::core::vector3df(0, 0, 0),
        irr::core::vector3df(0, -2, 0));
    cam->setFOV(M_PI / 2.0f);
    cam->setNearValue(0.1f);
    cam->setFarValue(10000.0f);
    then = device->getTimer()->getTime();
}

void Window::update() {
    while (device->run()) {
        if (device->isWindowActive()) {
            updateDeltaTime();
            handleEvent();
            driver->beginScene(true, true,
                irr::video::SColor(255, 100, 101, 140));

            smgr->drawAll();
            guienv->drawAll();

            driver->endScene();
        } else {
            device->yield();
        }
    }
    device->drop();
}

void Window::setupWorld() {
    if (cubes.size() > 0) {
        for (auto &cube : cubes) {
            cube->remove();
        }
        cubes.clear();
    }
    int width = GUI::GameDataManager::i().getWidth();
    int height = GUI::GameDataManager::i().getHeight();

    float deltaWidth = (width % 2 == 0) ? 0.5f : 0;
    float deltaHeight = (height % 2 == 0) ? 0.5f : 0;

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            irr::core::vector3df position(i - (width/2) + deltaWidth, -2,
                j - (height/2) + deltaHeight);
            float rotation = std::rand() % 4;
            auto cube = importMesh("Plane", position,
                irr::core::vector3df(0.45f),
                irr::core::vector3df(0, rotation * 90, 0));
            GameTile &tile = GUI::GameDataManager::i().addTile(i, j);
            tile.setTileMesh(cube);
        }
    }
}
}  // namespace GUI
