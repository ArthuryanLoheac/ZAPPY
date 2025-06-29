#include <vector>
#include <string>
#include <memory>
#include <queue>

#include "DataManager/GameDataManager.hpp"
#include "Exceptions/GraphicalExceptions.hpp"
#include "Exceptions/DataManagerExceptions.hpp"
#include "Window/window.hpp"

namespace GUI {

void GameDataManager::clear() {
    width = -1;
    height = -1;
    tiles.clear();
    eggs.clear();
    teams.clear();
    players.clear();
    playerAdded = false;
    eggAdded = false;
    playerDead = false;
    eggDead = false;
    elevation = false;
    Collecting = false;
    Dropping = false;
    Pushed = false;
    messages.clear();
    messagesThisFrame.clear();
}

int GameDataManager::getWidth() const {
    return width;
}

bool GameDataManager::getGameOver() const {
    return isGameOver;
}

void GameDataManager::setGameOver(bool over) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    isGameOver = over;
}

const std::string &GameDataManager::getWinner() {
    return winner;
}

void GameDataManager::setWinner(const std::string &winnerName) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    winner = winnerName;
}

int GameDataManager::getHeight() const {
    return height;
}

void GameDataManager::setWidth(int w) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    if (w < 0)
        throw std::invalid_argument("Width must be non-negative");
    width = w;
}

void GameDataManager::setHeight(int h) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    if (h < 0)
        throw std::invalid_argument("Height must be non-negative");
    height = h;
}

GameTile &GameDataManager::addTile(int x, int y) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    tiles.push_back(GameTile(x, y));
    return tiles.back();
}

GameTile &GameDataManager::getTile(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        throw std::out_of_range("Tile coordinates out of bounds " +
            std::to_string(x) + " / " + std::to_string(y));
    for (auto &tile : tiles) {
        if (tile.getX() == x && tile.getY() == y)
            return tile;
    }
    throw std::runtime_error("Tile " + std::to_string(x) +
        "/" + std::to_string(y) + " not found");
}
void GameDataManager::addTeam(const std::string &teamName) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    teams.push_back(teamName);
}

std::vector<Egg> &GameDataManager::getEggs() {
    return eggs;
}

void GameDataManager::addMessage(const std::string &message, int id) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    messages.emplace_back(message, id);
    messagesThisFrame.emplace_back(message, id);
    if (messages.size() > 5)
        messages.erase(messages.begin());
}

std::vector<GameDataManager::Message> &GameDataManager::getMessages() {
    return messages;
}

std::vector<GameDataManager::Message> &GameDataManager::getMessagesThisFrame() {
    return messagesThisFrame;
}

void GameDataManager::addEgg(int id, int team, int x, int y) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    Vec3d position = getTile(x, y).getWorldPos();
    position.Y += 0.2f;
    eggs.emplace_back(id, team, x, y, nullptr);
}

void GameDataManager::removeEgg(int id) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    for (size_t i = 0; i < eggs.size(); i++) {
        if (eggs[i].id == id) {
            eggs[i].isDead = true;
        }
    }
    throw ParseException("Invalid ID egg");
}

bool GameDataManager::isPlayerAdded() const {
    return playerAdded;
}

void GameDataManager::setPlayerAdded(bool added) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    playerAdded = added;
}

void GameDataManager::addPlayer(int id, int x, int y,
Player::Orientation o, int level, const std::string &teamName) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    Vec3d position = getTile(x, y).getWorldPos();
    position.Y += 0.5f;
    players.emplace_back(id, x, y, o, level, teamName, nullptr);
    playerAdded = true;
}

Player &GameDataManager::getPlayer(int id) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    for (auto &player : players) {
        if (player.getId() == id)
            return player;
    }
    throw ParseException("Player with this ID not found");
}

std::vector<Player> &GameDataManager::getPlayers() {
    return players;
}

void GameDataManager::removePlayer(int id) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    for (size_t i = 0; i < players.size(); i++) {
        if (players[i].getId() == id) {
            players[i].isDead = true;
            return;
        }
    }
    throw ParseException("Invalid ID player");
}

const std::vector<std::string> &GameDataManager::getTeams() const {
    return teams;
}

void GameDataManager::Update(float deltaTime) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    for (auto &player : players)
        player.Update(deltaTime);
}

void GameDataManager::setEggAdded(bool added) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    eggAdded = added;
}

bool GameDataManager::isEggAdded() const {
    return eggAdded;
}

bool GameDataManager::isPlayerDead() const {
    return playerDead;
}

void GameDataManager::setPlayerDead(bool dead) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    playerDead = dead;
}

bool GameDataManager::isEggDead() const {
    return eggDead;
}

void GameDataManager::setEggDead(bool dead) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    eggDead = dead;
}

bool GameDataManager::isElevation() const {
    return elevation;
}

void GameDataManager::setElevationSound(bool elev) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    elevation = elev;
}

bool GameDataManager::isCollecting() const {
    return Collecting;
}

void GameDataManager::setCollecting(bool collecting) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    Collecting = collecting;
}

bool GameDataManager::isDropping() const {
    return Dropping;
}

void GameDataManager::setDropping(bool dropping) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    Dropping = dropping;
}

bool GameDataManager::isPushed() const {
    return Pushed;
}

void GameDataManager::setPushed(bool pushed) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    Pushed = pushed;
}
}  // namespace GUI
