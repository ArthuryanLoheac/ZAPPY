#include "Graphic/Window/window.hpp"

namespace GUI {
Window::Window() {
    device = irr::createDevice(irr::video::EDT_OPENGL,
        irr::core::dimension2d<irr::u32>(1280, 720), 16, false, true, false, 0);

    if (!device)
        throw GUI::WindowCreationException("Error creating device");
    device->setWindowCaption(L"Zappy");
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
    // Camera
    auto cam = smgr->addCameraSceneNodeFPS();
    cam->setFOV(M_PI / 2.0f);
    cam->setNearValue(0.1f);
    cam->setFarValue(10000.0f);
}

void Window::update() {
    while (device->run()) {
        if (device->isWindowActive()) {
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

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            auto cube = smgr->addCubeSceneNode(0.8f, 0, -1,
                irr::core::vector3df(i - (width/2), -3, -j  + (height/2)));
            cube->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        }
    }
}

}  // namespace GUI
