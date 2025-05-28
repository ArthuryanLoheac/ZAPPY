#include <iostream>
#include <irrlicht/irrlicht.h>

#include "Exceptions/GraphicalExceptions.hpp"
#include "Graphic/Window/window.hpp"

int graphic() {
    try {
        Window window;
        window.update();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 84;
    }
    return 0;
}
