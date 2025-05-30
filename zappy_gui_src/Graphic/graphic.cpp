#include <irrlicht/irrlicht.h>

#include <iostream>

#include "Exceptions/GraphicalExceptions.hpp"
#include "Graphic/Window/window.hpp"
#include "DataManager/DataManager.hpp"

int graphic() {
    try {
        GUI::Window::i();
        GUI::Window::i().update();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 84;
    }
    GUI::DataManager::i().setRunning(false);
    return 0;
}
