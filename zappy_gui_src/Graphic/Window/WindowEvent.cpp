#include <irrlicht/irrlicht.h>
#include "MyEventReceiver.hpp"
#include "window.hpp"

namespace GUI {
void Window::handleEvent() {
    int xMoveCam = 0;

    if (receiver.IsKeyDown(irr::KEY_ESCAPE))
        device->closeDevice();
    if (receiver.IsKeyDown(irr::KEY_KEY_D))
        xMoveCam += 1;
    if (receiver.IsKeyDown(irr::KEY_KEY_Q))
        xMoveCam -= 1;
    moveCamera(xMoveCam);
}

void Window::updateDeltaTime() {
    const irr::u32 now = device->getTimer()->getTime();
    frameDeltaTime = (irr::f32)(now - then) / 1000.f; // Time in seconds
    then = now;
}

void Window::moveCamera(float x) {
    angleXCamera += x * frameDeltaTime * rotationSpeedCamera;
    if (angleXCamera > 360.f)
        angleXCamera -= 360.f;
    if (angleXCamera < 0.f)
        angleXCamera += 360.f;

    irr::core::vector3df pos(
        distanceFromCenter * std::cos(angleXCamera * (M_PI / 180.0f)),
        1,
        distanceFromCenter * std::sin(angleXCamera * (M_PI / 180.0f))
    );
    cam->setPosition(pos);
}

} // namespace GUI