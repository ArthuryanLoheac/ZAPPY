#include <memory>
#include <string>
#include <mutex>
#include <utility>

#include "DataManager/Player.hpp"
#include "DataManager/GameDataManager.hpp"
#include "DataManager/DataManager.hpp"
#include "Graphic/Window/window.hpp"

namespace GUI {
Player::Player(int id, int x, int y, Orientation o, int l,
const std::string &team, const std::shared_ptr<Mesh> &pMesh)
: id(id), x(x), y(y), o(o), level(l), teamName(team), PlayerMesh(pMesh) {
    Init(team, l);
}

Player::Player(Player &&other) noexcept
: id(other.id), x(other.x), y(other.y),
posTarget(std::move(other.posTarget)),
rotationTarget(std::move(other.rotationTarget)),
speedMove(other.speedMove),
baseSpeedMove(other.baseSpeedMove),
o(other.o), level(other.level),
teamName(std::move(other.teamName)),
PlayerMesh(std::move(other.PlayerMesh)),
PlayerMeshesCylinder(std::move(other.PlayerMeshesCylinder)),
PlayerMeshesCylinderRotation(std::move(other.PlayerMeshesCylinderRotation)),
state(other.state),
ressources(std::move(other.ressources)) {}

Player &Player::operator=(Player &&other) noexcept {
    if (this != &other) {
        std::lock_guard<std::mutex> lock(mutexDatas);
        id = other.id;
        x = other.x;
        y = other.y;
        posTarget = std::move(other.posTarget);
        rotationTarget = std::move(other.rotationTarget);
        speedMove = other.speedMove;
        baseSpeedMove = other.baseSpeedMove;
        o = other.o;
        level = other.level;
        teamName = std::move(other.teamName);
        PlayerMesh = std::move(other.PlayerMesh);
        PlayerMeshesCylinder = std::move(other.PlayerMeshesCylinder);
        PlayerMeshesCylinderRotation =
            std::move(other.PlayerMeshesCylinderRotation);
        ressources = std::move(other.ressources);
        timeTT = other.timeTT;
        state = other.state;
    }
    return *this;
}

float randRotation(int i) {
    i++;
    float r = random() % static_cast<int>(360 / i);
    return r * 2;
}

void Player::Init(std::string team, int level) {
    state = MOVING;
    timeTT = 0;
    (void) team;
    (void) level;
    for (int i = 0; i < maxLevel; i++) {
        PlayerMeshesCylinderRotation.push_back(Vec3d(randRotation(i),
            randRotation(i), randRotation(i)));
    }
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
        PlayerMeshesCylinder[i]->setVisible((i + 1) <=
            static_cast<size_t>(level));
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
    setPosition(newX, newY, new0, false);
}

void Player::setPosition(int newX, int newY, Orientation new0, bool TP) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    bool tp = false;
    if ((x == 0 && newX == GameDataManager::i().getWidth() - 1) ||
        (y == 0 && newY == GameDataManager::i().getHeight() - 1) ||
        (x == GameDataManager::i().getWidth() - 1 && newX == 0) ||
        (y == GameDataManager::i().getHeight() - 1 && newY == 0))
        tp = true;
    x = newX;
    y = newY;
    o = new0;

    if (isElevation)
        return;

    if (PlayerMesh) {
        Vec3d position = GameDataManager::i().getTile(x, y).getWorldPos();
        if (position.X == 0 && position.Z == 0 && position.Y == 0)
            position = PlayerMesh->getPosition();
        position.Y += 0.5f;
        posTarget = Vec3d(position.X, position.Y, position.Z);
        speedMove = baseSpeedMove * DataManager::i().getFrequency() *
            (posTarget - PlayerMesh->getPosition()).getLength();
        rotationTarget = Vec3d(0, o * 90, 0);
        // check first set
        if (PlayerMesh->getPosition().Y == 0 || tp || TP) {
            PlayerMesh->setPosition(position);
            posTarget = Vec3d(position.X, position.Y, position.Z);
            PlayerMesh->setRotation(Vec3d(0, o * 90, 0));
            for (size_t i = 0; i < PlayerMeshesCylinder.size(); i++)
                PlayerMeshesCylinder[i]->setPosition(position);
        }
    }
    GUI::Window::i().needUpdatePlayers = true;
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
    if (!PlayerMesh)
        return;
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
