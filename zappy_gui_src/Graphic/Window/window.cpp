#include "window.hpp"

Window::Window() {
    device = irr::createDevice(irr::video::EDT_SOFTWARE,
        irr::core::dimension2d<irr::u32>(640, 480), 16, false, true, false, 0);
    device->setWindowCaption(L"Zappy");

    if (!device)
        throw GUI::WindowCreationException("Error creating device");
}
