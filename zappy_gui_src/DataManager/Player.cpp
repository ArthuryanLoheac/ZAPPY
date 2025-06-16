#include <memory>
#include <string>
#include <mutex>
#include <utility>

#include "DataManager/Player.hpp"
#include "DataManager/GameDataManager.hpp"
#include "DataManager/DataManager.hpp"
#include "Graphic/Window/window.hpp"
#include "Player.hpp"

namespace GUI {

bool Player::checkAngleDiff(irr::core::vector3df a, irr::core::vector3df b) {
    float angleDiff = std::abs(a.Y - b.Y);
    if (angleDiff > 180.0f)
        angleDiff = 360.0f - angleDiff;
    return angleDiff > 5.0f;
}

void Player::Update(float deltaTime) {
    timeTT += deltaTime;
    if (state == MOVING) {
        UpdateMoving(deltaTime);
    } else if (state == START_ELEVATION) {
        updateStartElevation(deltaTime);
    } else if (state == IDLE_ELEVATION) {
        updateElevation(deltaTime);
    }
}

void Player::setElevation(bool isStart) {
    state = START_ELEVATION;
    isElevation = isStart;
    Vec3d targetRot = Vec3d(90, 0, 0);
    speed = 0.1f;
    Vec3d currentRot = PlayerMesh->getRotation();
    deltaRotPlayer = targetRot - currentRot;
    (void)isStart;
}

void Player::UpdateMoving(float deltaTime)
{
    updateRotation(deltaTime);
    updatePosition(deltaTime);
    updtaeIdle(deltaTime);
}

void Player::updateStartElevation(float deltaTime) {
    Vec3d currentRot = PlayerMesh->getRotation();
    Vec3d newRot = Vec3d(
        currentRot.X + deltaRotPlayer.X * deltaTime * 2,
        currentRot.Y + deltaRotPlayer.Y * deltaTime * 2,
        currentRot.Z + deltaRotPlayer.Z * deltaTime * 2);
    PlayerMesh->setRotation(newRot);
    if (currentRot.X >= 85.f && currentRot.X <= 95.f) {
        state = IDLE_ELEVATION;
        PlayerMesh->setRotation(Vec3d(90, 0, 0));
    }
}

void Player::updateElevation(float deltaTime) {
    speed += 0.05f * DataManager::i().getFrequency() * deltaTime;
    Vec3d rot = PlayerMesh->getRotation();
    rot = Vec3d(90, rot.Y + 90 * deltaTime * speed, 0);
    PlayerMesh->setRotation(rot);
    for (size_t i = 0; i < PlayerMeshesCylinder.size(); i++) {
        if (PlayerMeshesCylinder[i]) {
            Vec3d rotC = PlayerMeshesCylinder[i]->getRotation();
            PlayerMeshesCylinder[i]->setRotation(
                Vec3d(rotC.X, rotC.Y + 90 * deltaTime * speed, rotC.Z));
        }
    }
}

void Player::updateRotation(float deltaTime) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    for (size_t i = 0; i < PlayerMeshesCylinder.size(); i++) {
        if (PlayerMeshesCylinder[i]) {
            Vec3d rot = PlayerMeshesCylinder[i]->getRotation();
            PlayerMeshesCylinder[i]->setRotation(
                Vec3d(rot.X + PlayerMeshesCylinderRotation[i].X * deltaTime,
                      rot.Y + PlayerMeshesCylinderRotation[i].Y * deltaTime,
                      rot.Z + PlayerMeshesCylinderRotation[i].Z * deltaTime));
        }
    }
}

void Player::updatePosition(float deltaTime) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    float speedRotate = 15 * DataManager::i().getFrequency();

    if (posTarget.getDistanceFrom(PlayerMesh->getPosition()) > 0.1f) {
        // new pos
        Vec3d direction = posTarget - PlayerMesh->getPosition();
        direction.normalize();
        Vec3d newPos = PlayerMesh->getPosition() +
            (direction * speedMove * deltaTime);
        PlayerMesh->setPosition(newPos);
        // ring update
        for (size_t i = 0; i < PlayerMeshesCylinder.size(); i++) {
            if (PlayerMeshesCylinder[i])
                PlayerMeshesCylinder[i]->setPosition(newPos);
        }
    } else {
        // close enough to target position
        Vec3d pos = PlayerMesh->getPosition();
        pos.Y = GameDataManager::i().getTile(x, y).getWorldPos().Y + 0.5f;
        PlayerMesh->setPosition(pos);
        for (size_t i = 0; i < PlayerMeshesCylinder.size(); i++) {
            if (PlayerMeshesCylinder[i])
                PlayerMeshesCylinder[i]->setPosition(pos);
        }
    }
    // Update rotation
    if (checkAngleDiff(PlayerMesh->getRotation(), rotationTarget)) {
        float currentY = fmod(PlayerMesh->getRotation().Y, 360.0f);
        if (currentY < 0) currentY += 360.0f;
        float targetY = fmod(rotationTarget.Y, 360.0f);
        if (targetY < 0) targetY += 360.0f;

        float diff = targetY - currentY;
        if (diff > 180.0f) diff -= 360.0f;
        if (diff < -180.0f) diff += 360.0f;

        float step = speedRotate * deltaTime;
        if (abs(diff) < step) step = abs(diff);

        Vec3d currentRotation = PlayerMesh->getRotation();
        currentRotation.Y += diff > 0 ? step : -step;
        PlayerMesh->setRotation(currentRotation);
    }
}

void Player::updtaeIdle(float deltaTime) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    (void)deltaTime;
    float Newy = GameDataManager::i().getTile(x, y).getWorldPos().Y;
    Vec3d pos = PlayerMesh->getPosition();

    idlePosY = std::sin(timeTT * 3) * 0.05f + 0.5f;
    Newy += idlePosY;
    pos.Y = Newy;
    PlayerMesh->setPosition(pos);

    for (int i = 0; i < static_cast<int>(PlayerMeshesCylinder.size()); i++) {
        if (PlayerMeshesCylinder[i]) {
            Vec3d posC = PlayerMesh->getPosition();
            PlayerMeshesCylinder[i]->setPosition(posC);
        }
    }
}
}  // namespace GUI

