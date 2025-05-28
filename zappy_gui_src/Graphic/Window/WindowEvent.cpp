#include <irrlicht/irrlicht.h>
#include "MyEventReceiver.hpp"
#include "window.hpp"

namespace GUI {
void Window::handleEvent() {
    if (receiver.IsKeyDown(irr::KEY_ESCAPE))
        device->closeDevice();
}
}  // namespace GUI