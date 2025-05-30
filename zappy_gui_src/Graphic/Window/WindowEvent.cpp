#include <irrlicht/irrlicht.h>

#include <algorithm>

#include "Graphic/Events/MyEventReceiver.hpp"
#include "Graphic/Window/window.hpp"

namespace GUI {
void Window::handleEvent() {
    int xMoveCam = receiver.getValBetween(irr::KEY_KEY_E, irr::KEY_KEY_A);
    int yMoveCenterCam = receiver.getValBetween(irr::KEY_KEY_D, irr::KEY_KEY_Q);
    int xMoveCenterCam = receiver.getValBetween(irr::KEY_KEY_S, irr::KEY_KEY_Z);
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
    // rotate around
    updateRotation(x);
    float radAngleX = std::cos(angleXCamera * (M_PI / 180.0f));
    float radAngleZ = std::sin(angleXCamera * (M_PI / 180.0f));
    updateMoveOrigin(xMove, yMove, radAngleX, radAngleZ);
    updateZoomCamera(zoom);

    irr::core::vector3df pos(
        distanceFromCenter * radAngleX, 1,
        distanceFromCenter * radAngleZ);
    cam->setPosition(pos);
}

void Window::updateZoomCamera(float zoom) {
    // Zoom
    distanceFromCenter += zoom * frameDeltaTime * zoomSpeedCamera;
    distanceFromCenter = std::clamp(distanceFromCenter, 1.f, 50.f);
}

void Window::updateMoveOrigin(float xMove, float yMove, float radX,
float radZ) {
    int width = GUI::GameDataManager::i().getWidth();
    int height = GUI::GameDataManager::i().getHeight();

    irr::core::vector3df posTarget = cam->getTarget();
    // Rotate X & Y by the camera rotation
    float rotatedXMove = xMove * radX - yMove * radZ;
    float rotatedYMove = xMove * radZ + yMove * radX;
    xMove = rotatedXMove;
    yMove = rotatedYMove;
    // Move origin
    posTarget.X += xMove * frameDeltaTime * moveSpeedCamera;
    posTarget.Z += yMove * frameDeltaTime * moveSpeedCamera;
    posTarget.X = std::clamp(posTarget.X, -(width / 2.f), (width / 2.f));
    posTarget.Z = std::clamp(posTarget.Z, -(height / 2.f), (height / 2.f));
    cam->setTarget(posTarget);
}

void Window::updateRotation(float x) {
    angleXCamera += x * frameDeltaTime * rotationSpeedCamera;
    if (angleXCamera > 360.f) angleXCamera -= 360.f;
    if (angleXCamera < 0.f) angleXCamera += 360.f;
}
}  // namespace GUI
