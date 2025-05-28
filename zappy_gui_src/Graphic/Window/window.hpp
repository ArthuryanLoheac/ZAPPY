#pragma once
#include <irrlicht/irrlicht.h>
#include "Exceptions/GraphicalExceptions.hpp"

class Window {
 public:
    Window();
    irr::IrrlichtDevice *device;
};

