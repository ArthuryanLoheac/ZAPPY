#include <iostream>
#include <memory>
#include <random>
#include <string>

#include "Graphic/Window/window.hpp"
#include "Graphic/Events/MyEventReceiver.hpp"
#include "tools/MeshImporter.hpp"
#include "DataManager/DataManager.hpp"
#include "DataManager/SoundsManager.hpp"
#include "include/logs.h"
#include "PluginsManagement/PluginsDataManager.hpp"
#include "DataManager/PathManager.hpp"
#include "Window/windowOptionMenu.hpp"
#include "Connection/NetworkForGui.hpp"

namespace GUI {
void Window::SetupSkybox() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randRot(0, 359);
    std::uniform_int_distribution<> randSmall(0, 9);

    Skybox = std::shared_ptr<irr::scene::ISceneNode>(
        smgr->addSkyBoxSceneNode(
            driver->getTexture(("assets/" +
            GUI::PathManager::i().getPath("skyboxTop")).c_str()),
            driver->getTexture(("assets/" +
            GUI::PathManager::i().getPath("skyboxBottom")).c_str()),
            driver->getTexture(("assets/" +
            GUI::PathManager::i().getPath("skyboxFront")).c_str()),
            driver->getTexture(("assets/" +
            GUI::PathManager::i().getPath("skyboxBack")).c_str()),
            driver->getTexture(("assets/" +
            GUI::PathManager::i().getPath("skyboxRight")).c_str()),
            driver->getTexture(("assets/" +
            GUI::PathManager::i().getPath("skyboxLeft")).c_str())),
        [](irr::scene::ISceneNode *) {});
    rotationSkybox = Vec3d((randSmall(gen) - 5.f) / 10.f,
        (randSmall(gen) - 5.f) / 10.f, (randSmall(gen) - 5.f) / 10.f)
        * speedRotationSkybox;
    Skybox->setRotation(Vec3d(randRot(gen), randRot(gen), randRot(gen))
        * speedRotationSkybox);
}

Window::Window() {
    bool optimized = GUI::DataManager::i().isOptimized();
    device = irr::createDevice(
        optimized ? irr::video::EDT_OPENGL : irr::video::EDT_BURNINGSVIDEO,
        irr::core::dimension2d<irr::u32>(1280, 720), 16, false, false, false,
        &receiver);

    if (!device)
        throw GUI::WindowCreationException("Error creating device");
    device->setWindowCaption(L"Zappy");
    windowOptionMenu::i().device = device;
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
    driver->beginScene(true, true,
            irr::video::SColor(0, 0, 0, 0));
    driver->endScene();
}

void Window::updateSkyBoxRotation() {
    if (!Skybox)
        return;
    irr::core::vector3df rotation = Skybox->getRotation();
    rotation += rotationSkybox * frameDeltaTime * speedRotationSkybox;
    if (rotation.Y > 360.f) rotation.Y -= 360.f;
    Skybox->setRotation(rotation);
}

void Window::windowUpdateFocus() {
    handleEvent();
    updateSkyBoxRotation();
    GameDataManager::i().Update(frameDeltaTime);
    PluginsDataManager::i().updatePluginsData();
    pluginsManager::i().update(PluginsDataManager::i().getData(),
        frameDeltaTime);
    SoundsManager::i().Update();
    driver->beginScene(true, true,
        irr::video::SColor(255, 100, 101, 140));
    updateMesh();
    smgr->drawAll();
    windowOptionMenu::i().draw(driver, font);
    pluginsManager::i().drawPlugins(font, driver);
    guienv->drawAll();
}

void Window::windowUpdateNoFocus() {
    try {
        updateSkyBoxRotation();
        GameDataManager::i().Update(frameDeltaTime);
        SoundsManager::i().Update();
        PluginsDataManager::i().updatePluginsData();
        pluginsManager::i().update(PluginsDataManager::i().getData(),
            frameDeltaTime);
        driver->beginScene(true, true,
            irr::video::SColor(255, 100, 101, 140));

        updateMesh();
        smgr->drawAll();
        windowOptionMenu::i().draw(driver, font);
        pluginsManager::i().drawPlugins(font, driver);
        guienv->drawAll();
    } catch (const std::exception &e) {
        LOG_ERROR(("Error in windowUpdateNoFocus: " +
            std::string(e.what())).c_str());
    }
}

void Window::setUpdatePlayer(bool b) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    needUpdatePlayers = b;
}

void Window::setRotationSpeedSkybox(float speed) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    speedRotationSkybox = speed;
}

void Window::update() {
    while (device->run()) {
        if (GUI::NetworkForGui::i().toClear) {
            GUI::NetworkForGui::i().toClear = false;
            smgr->clear();
            cam = smgr->addCameraSceneNode(nullptr,
                irr::core::vector3df(0, 0, 0),
                irr::core::vector3df(0, -2, 0));
            cam->setFOV(M_PI / 2.0f);
            cam->setNearValue(0.1f);
            cam->setFarValue(10000.0f);
            SetupSkybox();
        }
        updateDeltaTime();
        if (device->isWindowActive())
            windowUpdateFocus();
        else
            windowUpdateNoFocus();
        driver->endScene();
    }
    device->drop();
}

void Window::clearMeshes() {
    for (auto &cube : cubes)
        cube->remove();
    cubes.clear();
    for (int i = 0; i < GUI::GameDataManager::i().getWidth(); i++) {
        for (int j = 0; j < GUI::GameDataManager::i().getHeight(); j++) {
            GameTile &tile = GUI::GameDataManager::i().getTile(i, j);
            tile.clear(smgr);
        }
    }
    if (light) {
        light->remove();
        light = nullptr;
    }
    for (auto &egg : GUI::GameDataManager::i().getEggs()) {
        if (egg.EggMesh) {
            egg.EggMesh->remove();
            egg.EggMesh = nullptr;
        }
    }
    for (auto &player : GUI::GameDataManager::i().getPlayers()) {
        player.clear(smgr);
    }
    worldSetuped = false;
}

void Window::setupWorldData() {
    if (cubes.size() > 0) {
        for (auto &cube : cubes) {
            cube->remove();
        }
        cubes.clear();
    }
    int width = GUI::GameDataManager::i().getWidth();
    int height = GUI::GameDataManager::i().getHeight();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            GUI::GameDataManager::i().addTile(i, j);
        }
    }
}

void Window::updateMesh() {
    for (size_t i = 0; i < GUI::GameDataManager::i().getPlayers().size(); i++) {
        if (GUI::GameDataManager::i().getPlayers()[i].isDead) {
            if (GUI::GameDataManager::i().getPlayers()[i].getMesh()) {
                GUI::GameDataManager::i().getPlayers()[i].getMesh()->remove();
                GUI::GameDataManager::i().getPlayers()[i].getMesh() = nullptr;
                for (auto &mesh : GUI::GameDataManager::i()
                    .getPlayers()[i].getPlayerMeshesCylinder()) {
                    mesh->remove();
                }
                GUI::GameDataManager::i().getPlayers()[i].
                    getPlayerMeshesCylinder().clear();
                GUI::GameDataManager::i().getPlayers()[i].
                    setMesh(nullptr);
                GUI::GameDataManager::i().getPlayers().erase(
                    GUI::GameDataManager::i().getPlayers().begin() + i);
                i--;
            }
        }
    }
    for (size_t i = 0; i < GUI::GameDataManager::i().getEggs().size(); i++) {
        if (GUI::GameDataManager::i().getEggs()[i].isDead) {
            if (GUI::GameDataManager::i().getEggs()[i].EggMesh) {
                GUI::GameDataManager::i().getEggs()[i].EggMesh->remove();
                GUI::GameDataManager::i().getEggs()[i].EggMesh = nullptr;
                GUI::GameDataManager::i().getEggs().erase(
                    GUI::GameDataManager::i().getEggs().begin() + i);
                i--;
            }
        }
    }
    try {
        if (!GUI::GameDataManager::i().getTile(0, 0).getTileMesh())
            worldSetupMesh();
    } catch (const std::exception &e) {
        worldSetupMesh();
    }
    if (!worldSetuped)
        return;

    if (needUpdateRessources)
        initMeshRessources();

    if (needUpdatePlayers || missingPlayersInit.size() > 0)
        initMeshPlayers();

    if (needUpdateEggs || missingEggsInit.size() > 0)
        initMeshEggs();
}

void Window::initMeshRessources() {
    for (int i = 0; i < GUI::GameDataManager::i().getWidth(); i++) {
        for (int j = 0; j < GUI::GameDataManager::i().getHeight(); j++) {
            GameTile &tile = GUI::GameDataManager::i().getTile(i, j);
            if (tile.getTileMesh() && tile.getTileMesh()->getMesh()) {
                try {
                    tile.updateMeshesRessources();
                } catch (const std::exception &e) {
                    std::cerr << "Error updating tile resources: "
                        << e.what() << '\n';
                }
            } else {
                std::cerr << "Error: Invalid tile mesh at ("
                    << i << ", " << j << ")" << std::endl;
            }
        }
    }
    needUpdateRessources = false;
}

void Window::removePlayerInitLst(int id) {
    for (auto it = missingPlayersInit.begin(); it
        != missingPlayersInit.end(); ++it) {
        if (*it == id) {
            missingPlayersInit.erase(it);
            return;
        }
    }
}

void Window::addPlayerInitLst(int id) {
    if (std::find(missingPlayersInit.begin(), missingPlayersInit.end(), id)
        == missingPlayersInit.end()) {
        missingPlayersInit.push_back(id);
    }
}

void Window::removeEggInitLst(int id) {
    for (auto it = missingEggsInit.begin(); it
        != missingEggsInit.end(); ++it) {
        if (*it == id) {
            missingEggsInit.erase(it);
            return;
        }
    }
}

void Window::addEggInitLst(int id) {
    if (std::find(missingEggsInit.begin(), missingEggsInit.end(), id)
        == missingEggsInit.end()) {
        missingEggsInit.push_back(id);
    }
}

void Window::initMeshPlayers() {
    std::lock_guard<std::mutex> lock(mutexDatas);
    for (auto &player : GUI::GameDataManager::i().getPlayers()) {
        if (!player.getMesh()) {
            Vec3d position = GUI::GameDataManager::i()
                .getTile(player.getX(), player.getY()).getWorldPos();
            position.Y += 0.5f;
            try {
                auto mesh = MeshImporter::i().importMesh(
                    PathManager::i().getPath("Player"), player.getTeamName(),
                    position, Vec3d(0.2f),
                    Vec3d(0, player.getOrientation() * 90, 0));
                if (mesh && mesh->getMesh()) {
                    player.setMesh(mesh);
                    player.initMeshRings();
                }
            } catch (const std::exception &e) {
                return;
            }
        } else if (player.getMesh()->getMesh()) {
            player.initMeshRings();
        }
    }
    needUpdatePlayers = false;
}

void Window::initMeshEggs() {
    std::lock_guard<std::mutex> lock(mutexDatas);
    for (auto &egg : GUI::GameDataManager::i().getEggs()) {
        if (!egg.EggMesh || !egg.EggMesh->getMesh()) {
            Vec3d position = GUI::GameDataManager::i().
                getTile(egg.x, egg.y).getWorldPos();
            position.Y += 0.2f;
            auto mesh = MeshImporter::i().importMesh(
                PathManager::i().getPath("Egg"), "",
                position, Vec3d(0.2f), Vec3d(0, 0, 0));
            if (mesh && mesh->getMesh()) {
                mesh->setVisible(!egg.isDead);
                egg.EggMesh = mesh;
                removeEggInitLst(egg.id);
            } else {
                LOG_ERROR(("Failed to create egg mesh for egg ID " +
                    std::to_string(egg.id)).c_str());
            }
        } else {
            LOG_DEBUG(("Egg mesh already exists for egg ID " +
                std::to_string(egg.id)).c_str());
        }
    }
    needUpdateEggs = false;
}

void Window::worldSetupMesh() {
    int width = GUI::GameDataManager::i().getWidth();
    int height = GUI::GameDataManager::i().getHeight();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            GameTile &tile = GUI::GameDataManager::i().getTile(i, j);
            if (!tile.getTileMesh()) {
                irr::core::vector3df position(i - (width/2) +
                    (width % 2 == 0 ? 0.5f : 0), -2,
                    j - (height/2) + (height % 2 == 0 ? 0.5f : 0));
                float rotation = std::rand() % 4;
                auto mesh = MeshImporter::i().importMesh(
                    PathManager::i().getPath("Tile"), "", position,
                    irr::core::vector3df(0.18f),
                    irr::core::vector3df(0, rotation * 90, 0));
                tile.setTileMesh(mesh);
            }
        }
    }
    light = smgr->addLightSceneNode(nullptr, irr::core::vector3df(30, 30, 0),
        GUI::PathManager::i().getLightColor(), 2000.0f);
    smgr->setAmbientLight(irr::video::SColorf(0.2f, 0.2f, 0.2f));
    worldSetuped = true;
    needUpdateRessources = true;
}
}  // namespace GUI
