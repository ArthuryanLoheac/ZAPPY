#include "window.hpp"

Window::Window() {
    device = irr::createDevice(irr::video::EDT_SOFTWARE,
        irr::core::dimension2d<irr::u32>(640, 480), 16, false, true, false, 0);

    if (!device)
        throw GUI::WindowCreationException("Error creating device");
    device->setWindowCaption(L"Zappy");
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();
    // Camera
    smgr->addCameraSceneNode(0, irr::core::vector3df(0,0,5),
        irr::core::vector3df(0,0,0));
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
