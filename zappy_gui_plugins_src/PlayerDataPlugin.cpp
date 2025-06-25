#include "zappy_gui_plugins_src/PlayerDataPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include "PlayerDataPlugin.hpp"

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

    LevelUpButton.blocked = player.level >= 8;

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
    // BUTTONS
    y += 20;
    LevelUpButton.updatePos(irr::core::position2d<irr::s32>(
        width - 200, y));
    ForwardButton.updatePos(irr::core::position2d<irr::s32>(
        width - 140, y + 40));
    BackwardButton.updatePos(irr::core::position2d<irr::s32>(
        width - 140, y + 80));
    LeftButton.updatePos(irr::core::position2d<irr::s32>(
        width - 180, y + 80));
    RightButton.updatePos(irr::core::position2d<irr::s32>(
        width - 100, y + 80));
    ForkButton.updatePos(irr::core::position2d<irr::s32>(
        width - 200, y + 120));
    EjectButton.updatePos(irr::core::position2d<irr::s32>(
        width - 200, y + 160));
    LevelUpButton.draw(font, driver, *this);
    ForwardButton.draw(font, driver, *this);
    BackwardButton.draw(font, driver, *this);
    LeftButton.draw(font, driver, *this);
    RightButton.draw(font, driver, *this);
    ForkButton.draw(font, driver, *this);
    EjectButton.draw(font, driver, *this);
}

void PlayerDataPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver* _driver) {
    driver = _driver;

    if (data.tiles.size() <= 0 || idPlayer == -1 || !font || !driver)
        return;
    int width = driver->getScreenSize().Width;
    try {
        drawImage("assets/UI/All.png", width - 240, 320, 250, 360,
            driver);
        drawPlayerInfo(idPlayer, font);
    } catch (std::exception &e) {}
}

bool PlayerDataPlugin::handleClick(std::string &outBuffer) {
    if (detectCollisionPlayer())
        return true;
    if (LevelUpButton.isHover(device)) {
        LevelUpButton.hover = false;
        outBuffer += "levelUp #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    if (ForwardButton.isHover(device)) {
        ForwardButton.hover = false;
        outBuffer += "d_forward #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    if (BackwardButton.isHover(device)) {
        BackwardButton.hover = false;
        outBuffer += "d_backward #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    if (LeftButton.isHover(device)) {
        LeftButton.hover = false;
        outBuffer += "d_right #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    if (RightButton.isHover(device)) {
        RightButton.hover = false;
        outBuffer += "d_left #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    if (ForkButton.isHover(device)) {
        ForkButton.hover = false;
        outBuffer += "d_fork #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    if (EjectButton.isHover(device)) {
        EjectButton.hover = false;
        outBuffer += "d_eject #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    return false;
}

bool PlayerDataPlugin::onEvent(const irr::SEvent &event, pluginsData datas,
std::string &outBuffer) {
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
        LevelUpButton.isHover(device);
        ForkButton.isHover(device);
        EjectButton.isHover(device);
        ForwardButton.isHover(device);
        BackwardButton.isHover(device);
        LeftButton.isHover(device);
        RightButton.isHover(device);

        if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
            pressed = true;
        } else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
            pressed = false;
        }
        if (pressed && !isPressedLastFrame) {
            if (handleClick(outBuffer))
                return true;
        }
        isPressedLastFrame = pressed;
    }
    (void) datas;
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

// ============ BUTTON ============

void PlayerDataPlugin::Button::draw(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver *driver, PlayerDataPlugin plug) {
    if (!font || !driver)
        return;
    plug.drawImage("assets/UI/AllRed.png", rect.UpperLeftCorner.X,
        rect.UpperLeftCorner.Y, rect.getWidth(), rect.getHeight(), driver,
        blocked ? 50 : (hover ? 120 : 255));
    font->draw(name.c_str(), rect, irr::video::SColor(
        blocked ? 100 : 255, 255, 255, 255), true, true);
}

bool PlayerDataPlugin::Button::isHover(irr::IrrlichtDevice *device) {
    hover = rect.isPointInside(device->getCursorControl()->getPosition());
    return hover;
}

void PlayerDataPlugin::Button::updatePos(
const irr::core::position2d<irr::s32> &pos) {
    rect = irr::core::rect<irr::s32>(pos.X, pos.Y,
        pos.X + rect.getWidth(), pos.Y + rect.getHeight());
}
