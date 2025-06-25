#include "zappy_gui_plugins_src/PlayerDataPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<PlayerDataPlugin>();
    }
}

pluginsData::Tile PlayerDataPlugin::getTile(int x, int y) {
    for (auto tile : data.tiles) {
        if (tile.x == x && tile.y == y)
            return tile;
    }
    throw std::out_of_range("Not Exsiting");
}

void PlayerDataPlugin::drawPlayerInfo(int id,
std::shared_ptr<irr::gui::IGUIFont> font) {
    int width = driver->getScreenSize().Width;
    pluginsData::Player player = data.getPlayer(id);
    int y = 350;

    // Name
    std::string playerInfo = "Player " + std::to_string(id) + " : " +
        player.teamName;
    font->draw(playerInfo.c_str(), UIRect(width - 220, y, 300, 300),
        player.color);
    // Level
    y += 20;
    playerInfo = "\tLevel : " + std::to_string(player.level)
        + (player.inElevation ? " (elevating)" : "");
    font->draw(playerInfo.c_str(), UIRect(width - 220, y, 300, 300),
        UICol(255, 255, 255, 255));
    // Position
    playerInfo = "\tPos : " + std::to_string(player.x) +
        ", " + std::to_string(player.y) + " - ";
    // Direction
    switch (player.orientation) {
        case pluginsData::Player::Orientation::NORTH:
            playerInfo += "North";
            break;
        case pluginsData::Player::Orientation::SOUTH:
            playerInfo += "South";
            break;
        case pluginsData::Player::Orientation::EAST:
            playerInfo += "East";
            break;
        case pluginsData::Player::Orientation::WEST:
            playerInfo += "West";
            break;
    }
    y += 20;
    font->draw(playerInfo.c_str(), UIRect(width - 220, y, 300, 300),
        UICol(255, 255, 255, 255));
    // Inventory
    y += 20;
    for (int i = 0; i < 7; ++i) {
        if (player.ressources[i] == 0)
            continue;
        playerInfo = "\t - " + lstNames[i] + " : " +
            std::to_string(player.ressources[i]);
        font->draw(playerInfo.c_str(), UIRect(width - 220, y, 300, 300),
            lstColors[i]);
        y += 20;
    }
}

void PlayerDataPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver* _driver) {
    driver = _driver;

    if (data.tiles.size() <= 0 || idPlayer == -1 || !font || !driver)
        return;
    int width = driver->getScreenSize().Width;
    try {
        drawImage("assets/UI/All.png", width - 240, 320, 250, 200,
            driver);
        drawPlayerInfo(idPlayer, font);
    } catch (std::exception &e) {}
}

bool PlayerDataPlugin::onEvent(const irr::SEvent &event, pluginsData datas, std::string &outBuffer) {
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
        if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
            pressed = true;
        } else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
            pressed = false;
        }
        if (pressed && !isPressedLastFrame)
            return detectCollisionPlayer();
        isPressedLastFrame = pressed;
    }
    (void) datas;
    (void) outBuffer;
    return false;
}

bool PlayerDataPlugin::detectCollisionPlayer() {
    if (data.players.size() <= 0)
        return false;
    irr::core::position2d<irr::s32> mousePos =
        device->getCursorControl()->getPosition();
    irr::core::line3d<irr::f32> ray = smgr->getSceneCollisionManager()
        ->getRayFromScreenCoordinates(mousePos, cam);

    for (auto &player : data.players) {
        if (!player.PlayerMesh)
            continue;
        irr::core::vector3df Pos = player.PlayerMesh->getPosition();
        irr::core::aabbox3d<irr::f32> box(Pos.X - 0.1f, Pos.Y - 0.1f,
            Pos.Z - 0.5f, Pos.X + 0.5f, Pos.Y + 0.1f, Pos.Z + 0.5f);
        if (box.intersectsWithLine(ray)) {
            if (idPlayer == player.id) {
                idPlayer = -1;
                return true;
            }
            idPlayer = player.id;
            return true;
        }
    }
    return false;
}
