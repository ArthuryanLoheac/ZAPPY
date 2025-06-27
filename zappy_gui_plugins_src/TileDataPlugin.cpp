#include "zappy_gui_plugins_src/TileDataPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <map>
#include <algorithm>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<TileDataPlugin>();
    }
}

pluginsData::Tile TileDataPlugin::getTile(int x, int y) {
    for (auto tile : data.tiles) {
        if (tile.x == x && tile.y == y)
            return tile;
    }
    throw std::out_of_range("Not Exsiting");
}

void TileDataPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver* driver) {
    int width = driver->getScreenSize().Width;
    int y = 30;

    if (!font || !driver || data.tiles.size() <= 0)
        return;
    this->font = font;
    this->driver = driver;

    if (data.tiles.size() <= 0 || (xTile == -1 && yTile == -1))
        return;
    try {
        pluginsData::Tile tile = getTile(xTile, yTile);
        drawImage("assets/UI/BottomLeft.png", width - 240, 0, 240, 300,
            driver);

        drawRessources(tile, y);
        y += 20;
        drawEggs(tile, y);
        y += 20;
        drawPlayers(tile, y);
    } catch (std::exception &e) {}
}

void TileDataPlugin::drawRessources(pluginsData::Tile tile, int &y) {
    int width = driver->getScreenSize().Width;

    std::string tileInfo = "Tile : " + std::to_string(tile.x) + ", " +
        std::to_string(tile.y) + " :";
    font->draw(tileInfo.c_str(), UIRect(width - 220, y, 300, 300),
        UICol(255, 255, 255, 255));
    y += 20;
    for (int i = 0; i < 7; ++i) {
        if (tile.resources[i] == 0)
            continue;
        tileInfo = "\t - " + lstNames[i] + " : " + std::to_string(
            tile.resources[i]);
        font->draw(tileInfo.c_str(), UIRect(width - 220, y, 300, 300),
            lstColors[i]);
        y += 20;
    }
}

void TileDataPlugin::drawEggs(pluginsData::Tile tile, int &y) {
    int width = driver->getScreenSize().Width;
    std::map<std::string, int> eggOwners;
    std::map<std::string, int> teamEgg;

    if (data.eggs.empty())
        return;
    for (const auto &egg : data.eggs) {
        if (egg.x == tile.x && egg.y == tile.y) {
            eggOwners[egg.owner == -1 ? "No team" : data.teams[egg.owner]]++;
            teamEgg[egg.owner == -1 ? "No team" : data.teams[egg.owner]]
                = egg.owner;
        }
    }
    if (eggOwners.empty())
        return;
    font->draw("Eggs :", UIRect(width - 220, y, 300, 300),
        UICol(255, 255, 255, 255));
    y += 20;
    for (const auto &egg : eggOwners) {
        std::string eggInfo = "\t - " + egg.first + " : " +
            std::to_string(egg.second);
        UICol eggColor = UICol(255, 255, 255, 255);
        if (egg.first != "No team") {
            eggColor = data.teamColors[teamEgg[egg.first]];
        }
        font->draw(eggInfo.c_str(), UIRect(width - 220, y, 300, 300), eggColor);
        y += 20;
    }
}

void TileDataPlugin::drawPlayers(pluginsData::Tile tile, int &y) {
    int width = driver->getScreenSize().Width;
    std::map<std::string, int> playerOwners;

    if (data.players.empty())
        return;
    for (const auto &player : data.players) {
        if (player.x == tile.x && player.y == tile.y) {
            playerOwners[player.teamName]++;
        }
    }
    if (playerOwners.empty())
        return;
    font->draw("Players :", UIRect(width - 220, y, 300, 300),
        UICol(255, 255, 255, 255));
    y += 20;
    for (const auto &player : playerOwners) {
        std::string playerInfo = "\t - " + player.first + " : " +
            std::to_string(player.second);
        UICol playerColor = UICol(255, 255, 255, 255);
        auto it = std::find(data.teams.begin(), data.teams.end(), player.first);
        if (it != data.teams.end()) {
            int index = std::distance(data.teams.begin(), it);
            playerColor = data.teamColors[index];
        }
        font->draw(playerInfo.c_str(),
            UIRect(width - 220, y, 300, 300), playerColor);
        y += 20;
    }
}

bool TileDataPlugin::onEvent(const irr::SEvent &event, pluginsData datas,
std::string &outBuffer) {
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
        if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
            pressed = true;
        } else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
            pressed = false;
        }
        if (pressed && !isPressedLastFrame)
            return detectCollisionTile();
        isPressedLastFrame = pressed;
    }
    (void) datas;
    (void) outBuffer;
    return false;
}

bool TileDataPlugin::detectCollisionTile() {
    irr::core::position2d<irr::s32> mousePos =
        device->getCursorControl()->getPosition();
    irr::core::line3d<irr::f32> ray = smgr->getSceneCollisionManager()
        ->getRayFromScreenCoordinates(mousePos, cam);

    irr::core::plane3d<irr::f32> groundPlane(irr::core::vector3df(0, -2, 0),
        irr::core::vector3df(0, 1, 0));
    irr::core::vector3df worldPos;

    if (groundPlane.getIntersectionWithLine(
        ray.start, ray.getVector(), worldPos)) {
        worldPos.X += data.width / 2.f;
        worldPos.Z += data.height / 2.f;
        int x = static_cast<int>(worldPos.X);
        int y = static_cast<int>(worldPos.Z);
        if (x < 0 || x >= data.width ||
            y < 0 || y >= data.height)
            return false;
        if (xTile == x && yTile == y) {
            xTile = -1;
            yTile = -1;
            return true;
        }
        xTile = x;
        yTile = y;
        return true;
    }
    return false;
}


int TileDataPlugin::countTileRessource(pluginsData::Tile tile) {
    int count = 0;

    for (int i = 0; i < 7; ++i) {
        if (tile.resources[i] == 0)
            continue;
        count++;
    }
    return count;
}

