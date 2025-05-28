#include <irrlicht/irrlicht.h>

#include <algorithm>

#include "Graphic/Events/MyEventReceiver.hpp"
#include "Graphic/Window/window.hpp"

namespace GUI {
void Window::handleEvent() {
    int xMoveCam = receiver.getValueTwoKey(irr::KEY_KEY_D, irr::KEY_KEY_Q);
    int yMoveCenterCam = receiver.getValueTwoKey(irr::KEY_RIGHT, irr::KEY_LEFT);
    int xMoveCenterCam = receiver.getValueTwoKey(irr::KEY_DOWN, irr::KEY_UP);
    float zoom = 0;

    if (receiver.IsKeyDown(irr::KEY_ESCAPE))
        device->closeDevice();
    zoom = -receiver.ConsumeWheelDelta();
    moveCamera(xMoveCam, zoom, xMoveCenterCam, yMoveCenterCam);
}

void Window::updateDeltaTime() {
    const irr::u32 now = device->getTimer()->getTime();
    frameDeltaTime = (irr::f32)(now - then) / 1000.f;
    then = now;
}

void Window::moveCamera(float x, float zoom, float xMove, float yMove) {
    // move Origin
    int width = GUI::GameDataManager::i().getWidth();
    int height = GUI::GameDataManager::i().getHeight();


    irr::core::vector3df posTarget = cam->getTarget();
    posTarget.X += xMove * frameDeltaTime * moveSpeedCamera;
    posTarget.Z += yMove * frameDeltaTime * moveSpeedCamera;
    posTarget.X = std::clamp(posTarget.X, -(width / 2.f), (width / 2.f));
    posTarget.Z = std::clamp(posTarget.Z, -(height / 2.f), (height / 2.f));
    cam->setTarget(posTarget);
    // rotate around
    angleXCamera += x * frameDeltaTime * rotationSpeedCamera;
    if (angleXCamera > 360.f) angleXCamera -= 360.f;
    if (angleXCamera < 0.f) angleXCamera += 360.f;
    irr::core::vector3df pos(
        distanceFromCenter * std::cos(angleXCamera * (M_PI / 180.0f)),
        1,
        distanceFromCenter * std::sin(angleXCamera * (M_PI / 180.0f)));
    // Zoom
    distanceFromCenter += zoom * frameDeltaTime * zoomSpeedCamera;
    distanceFromCenter = std::clamp(distanceFromCenter, 1.f, 50.f);
    cam->setPosition(pos);
}

}  // namespace GUI
