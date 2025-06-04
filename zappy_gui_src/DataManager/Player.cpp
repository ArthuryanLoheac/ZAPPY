#include <memory>
#include <string>
#include <mutex>

#include "DataManager/Player.hpp"

namespace GUI {

void Player::setId(int newId) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    id = newId;
}

int Player::getId() const {
    return id;
}

void Player::setX(int newX) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    x = newX;
}

int Player::getX() const {
    return x;
}

void Player::setY(int newY) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    y = newY;
}

int Player::getY() const {
    return y;
}

void Player::setOrientation(Orientation newO) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    o = newO;
}

Player::Orientation Player::getOrientation() const {
    return o;
}

void Player::setLevel(int newLevel) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    level = newLevel;
}

int Player::getLevel() const {
    return level;
}

void Player::setTeamName(const std::string &newTeamName) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    teamName = newTeamName;
}

const std::string &Player::getTeamName() const {
    return teamName;
}

void Player::setPosition(int newX, int newY) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    x = newX;
    y = newY;
}

void Player::setMesh(const std::shared_ptr<Mesh> &mesh) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    PlayerMesh = mesh;
}

std::shared_ptr<Mesh> Player::getMesh() const {
    return PlayerMesh;
}

}  // namespace GUI
