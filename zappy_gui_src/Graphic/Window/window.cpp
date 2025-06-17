#include <iostream>
#include <memory>
#include <random>

#include "Graphic/Window/window.hpp"
#include "Graphic/Events/MyEventReceiver.hpp"
#include "tools/MeshImporter.hpp"
#include "DataManager/DataManager.hpp"
#include "DataManager/SoundsManager.hpp"
#include "PluginsManagement/PluginsDataManager.hpp"

namespace GUI {
void Window::SetupSkybox() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randRot(0, 359);
    std::uniform_int_distribution<> randSmall(0, 9);

    Skybox = std::shared_ptr<irr::scene::ISceneNode>(
        smgr->addSkyBoxSceneNode(
            driver->getTexture("assets/skybox/top.png"),
            driver->getTexture("assets/skybox/bottom.png"),
            driver->getTexture("assets/skybox/back.png"),
            driver->getTexture("assets/skybox/front.png"),
            driver->getTexture("assets/skybox/left.png"),
            driver->getTexture("assets/skybox/right.png")),
        [](irr::scene::ISceneNode *) {});
    rotationSkybox = Vec3d((randSmall(gen) - 5.f) / 10.f,
        (randSmall(gen) - 5.f) / 10.f, (randSmall(gen) - 5.f) / 10.f);
    Skybox->setRotation(Vec3d(randRot(gen), randRot(gen), randRot(gen)));
}

Window::Window() {
    device = irr::createDevice(irr::video::EDT_BURNINGSVIDEO,
        irr::core::dimension2d<irr::u32>(1280, 720), 16, false, false, false,
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

    font = std::shared_ptr<irr::gui::IGUIFont>(
        guienv->getFont("assets/fonts/DejaVuSansMono.png"),
        [](irr::gui::IGUIFont *) {});
    SetupSkybox();
}

void Window::updateSkyBoxRotation() {
    if (!Skybox)
        return;
    irr::core::vector3df rotation = Skybox->getRotation();
    rotation += rotationSkybox * frameDeltaTime;
    if (rotation.Y > 360.f) rotation.Y -= 360.f;
    Skybox->setRotation(rotation);
}

void Window::windowUpdateFocus() {
    handleEvent();
    updateSkyBoxRotation();
    GameDataManager::i().Update(frameDeltaTime);
    PluginsDataManager::i().updatePluginsData();
    pluginsManager::i().update(PluginsDataManager::i().getData());
    SoundsManager::i().Update();
    driver->beginScene(true, true,
        irr::video::SColor(255, 100, 101, 140));

    smgr->drawAll();
    pluginsManager::i().drawPlugins(font, driver);
    guienv->drawAll();
}

void Window::windowUpdateNoFocus() {
    updateSkyBoxRotation();
    GameDataManager::i().Update(frameDeltaTime);
    SoundsManager::i().Update();
    driver->beginScene(true, true,
        irr::video::SColor(255, 100, 101, 140));

    smgr->drawAll();
    pluginsManager::i().drawPlugins(font, driver);
    guienv->drawAll();
}

void Window::update() {
    while (device->run()) {
        updateDeltaTime();
        if (device->isWindowActive())
            windowUpdateFocus();
        else
            windowUpdateNoFocus();
        driver->endScene();
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
            auto cube = MeshImporter::i().importMesh("Plane", "", position,
                irr::core::vector3df(0.18f),
                irr::core::vector3df(0, rotation * 90, 0));
            GameTile &tile = GUI::GameDataManager::i().addTile(i, j);
            tile.setTileMesh(cube);
        }
    }
    smgr->addLightSceneNode(nullptr, irr::core::vector3df(30, 30, 0),
        irr::video::SColorf(1.5f, 1.5f, 2.f), 2000.0f);
    smgr->setAmbientLight(irr::video::SColorf(0.2f, 0.2f, 0.2f));
}
}  // namespace GUI
