#include "window.hpp"

Window::Window() {
    device = irr::createDevice(irr::video::EDT_SOFTWARE,
        irr::core::dimension2d<irr::u32>(1280, 720), 16, false, true, false, 0);

    if (!device)
        throw GUI::WindowCreationException("Error creating device");
    device->setWindowCaption(L"Zappy");
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
    // Camera
    smgr->addCameraSceneNode(0, irr::core::vector3df(0,5,10),
        irr::core::vector3df(0,2,0));
    setupWorld();
}

void Window::update() {
    while(device->run()) {
        driver->beginScene(true, true, irr::video::SColor(255,100,101,140));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
    }
    device->drop();
}

void Window::setupWorld() {
    int width = GUI::GameDataManager::i().getWidth();
    int height = GUI::GameDataManager::i().getHeight();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            smgr->addCubeSceneNode(0.8f, 0, -1, irr::core::vector3df(i - (width/2), -1, -j  + (height/2)));
        }
    }
}
