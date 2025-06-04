#include <memory>
#include <string>
#include <mutex>
#include <utility>

#include "DataManager/Player.hpp"
#include "DataManager/GameDataManager.hpp"
#include "Player.hpp"
#include "Graphic/Window/window.hpp"

namespace GUI {

Player::Player(int id, int x, int y, Orientation o, int l,
const std::string &team, const std::shared_ptr<Mesh> &pMesh)
: id(id), x(x), y(y), o(o), level(l), teamName(team), PlayerMesh(pMesh) {
    for (int i = 0; i < 8; i++) {
        PlayerMeshesCylinder.push_back(
            std::shared_ptr<Mesh>(MeshImporter::i().importMesh("Cylinder", team)));
        PlayerMeshesCylinder[i]->setScale(Vec3d(0.2f + (0.04f * i)));
        PlayerMeshesCylinder[i]->setVisible((i + 1) <= l);
    }
}

Player::Player(Player &&other) noexcept
: id(other.id), x(other.x), y(other.y), o(other.o), level(other.level),
teamName(std::move(other.teamName)), PlayerMesh(std::move(other.PlayerMesh))  {
    for (int i = 0; i < 8; i++) {
        PlayerMeshesCylinder.push_back(
            std::shared_ptr<Mesh>(MeshImporter::i().importMesh("Cylinder", teamName)));
        PlayerMeshesCylinder[i]->setScale(Vec3d(0.2f + (0.04f * i)));
        PlayerMeshesCylinder[i]->setVisible((i + 1) <= level);
    }
}

Player &Player::operator=(Player &&other) noexcept {
    if (this != &other) {
        id = other.id;
        x = other.x;
        y = other.y;
        o = other.o;
        setLevel(other.level);
        teamName = std::move(other.teamName);
        PlayerMesh = std::move(other.PlayerMesh);
    }
    return *this;
}

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
    for (size_t i = 0; i < PlayerMeshesCylinder.size(); i++)
        PlayerMeshesCylinder[i]->setVisible((i + 1)  <= static_cast<size_t>(level));
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

void Player::setPosition(int newX, int newY, Orientation new0) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    x = newX;
    y = newY;
    o = new0;
    if (PlayerMesh) {
        Vec3d position = GameDataManager::i().getTile(x, y).getWorldPos();
        position.Y += 0.5f;
        PlayerMesh->setPosition(position);
        PlayerMesh->setRotation(Vec3d(0, o * 90, 0));
        for (size_t i = 0; i < PlayerMeshesCylinder.size(); i++) {
            PlayerMeshesCylinder[i]->setPosition(position);
            PlayerMeshesCylinder[i]->setRotation(Vec3d(0, o * 90, 0));
        }
    }
}

void Player::setPosition(int newX, int newY) {
    setPosition(newX, newY, o);
}

void Player::setMesh(const std::shared_ptr<Mesh> &mesh) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    PlayerMesh = mesh;
}

std::shared_ptr<Mesh> Player::getMesh() const {
    return PlayerMesh;
}

void Player::setRessource(int id, int value) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    if (id < 0 || id >= static_cast<int>(ressources.size()))
        throw std::out_of_range("Invalid resource ID : " + std::to_string(id));
    ressources[id] = value;
}

int Player::getRessource(int id) const {
    if (id < 0 || id >= static_cast<int>(ressources.size()))
        throw std::out_of_range("Invalid resource ID");
    return ressources[id];
}

void Player::destroy() {
    std::lock_guard<std::mutex> lock(mutexDatas);
    int idM = PlayerMesh->getID();
    auto sceneNode = GUI::Window::i().smgr->getSceneNodeFromId(idM);
    if (sceneNode)
        GUI::Window::i().smgr->addToDeletionQueue(sceneNode);
    for (size_t i = 0; i < PlayerMeshesCylinder.size(); i++) {
        int idMC = PlayerMeshesCylinder[i]->getID();
        auto sceneNodeC = GUI::Window::i().smgr->getSceneNodeFromId(idMC);
        if (sceneNodeC)
            GUI::Window::i().smgr->addToDeletionQueue(sceneNodeC);
    }
}

}  // namespace GUI

