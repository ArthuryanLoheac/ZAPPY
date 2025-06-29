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
    if (!font || id < 0 || data.players.empty())
        return;

    int width = driver->getScreenSize().Width;
    pluginsData::Player player = data.getPlayer(id);
    int y = 350;

    LevelUpButton.blocked = player.level >= 8;

    // Precompute player info strings
    std::string playerInfo = "Player " + std::to_string(id) + " : " +
        player.teamName;
    std::string levelInfo = "\tLevel : " + std::to_string(player.level) +
        (player.inElevation ? " (elevating)" : "");
    std::string positionInfo = "\tPos : " + std::to_string(player.x) + ", " +
        std::to_string(player.y) + " - ";

    // Direction
    static const std::string directions[] = {"North", "South", "East", "West"};
    positionInfo += directions[static_cast<int>(player.orientation)];

    // Draw player info
    font->draw(playerInfo.c_str(), UIRect(width - 220, y, 300, 300),
        player.color);
    y += 20;
    font->draw(levelInfo.c_str(), UIRect(width - 220, y, 300, 300),
        UICol(255, 255, 255, 255));
    y += 20;
    font->draw(positionInfo.c_str(), UIRect(width - 220, y, 300, 300),
        UICol(255, 255, 255, 255));

    // Inventory
    y += 20;
    for (int i = 0; i < 7; ++i) {
        if (player.ressources[i] > 0) {
            playerInfo = "\t - " + lstNames[i] + " : " +
                std::to_string(player.ressources[i]);
            font->draw(playerInfo.c_str(), UIRect(width - 220, y, 300, 300),
                lstColors[i]);
            y += 20;
        }
    }

    // BUTTONS
    LevelUpButton.updatePos({width - 200, y});
    ForwardButton.updatePos({width - 140, y + 40});
    BackwardButton.updatePos({width - 140, y + 80});
    LeftButton.updatePos({width - 180, y + 80});
    RightButton.updatePos({width - 100, y + 80});
    ForkButton.updatePos({width - 200, y + 120});
    EjectButton.updatePos({width - 200, y + 160});

    LevelUpButton.draw(font, driver);
    ForwardButton.draw(font, driver);
    BackwardButton.draw(font, driver);
    LeftButton.draw(font, driver);
    RightButton.draw(font, driver);
    ForkButton.draw(font, driver);
    EjectButton.draw(font, driver);
}

void PlayerDataPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver* _driver) {
    driver = _driver;

    if (data.tiles.empty() || idPlayer == -1 || !font || !driver)
        return;

    int width = driver->getScreenSize().Width;

    try {
        // Preload UI image once
        static const std::string uiImagePath = "assets/UI/All.png";
        drawImage(uiImagePath, width - 240, 320, 250, 360, driver);

        drawPlayerInfo(idPlayer, font);
    } catch (const std::exception &e) {
    }
}

bool PlayerDataPlugin::handleClick(std::string &outBuffer,
irr::core::vector2d<irr::s32> pos) {
    if (detectCollisionPlayer())
        return true;
    if (LevelUpButton.isHover(pos)) {
        LevelUpButton.hover = false;
        outBuffer += "levelUp #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    if (ForwardButton.isHover(pos)) {
        ForwardButton.hover = false;
        outBuffer += "d_forward #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    if (BackwardButton.isHover(pos)) {
        BackwardButton.hover = false;
        outBuffer += "d_backward #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    if (LeftButton.isHover(pos)) {
        LeftButton.hover = false;
        outBuffer += "d_right #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    if (RightButton.isHover(pos)) {
        RightButton.hover = false;
        outBuffer += "d_left #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    if (ForkButton.isHover(pos)) {
        ForkButton.hover = false;
        outBuffer += "d_fork #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    if (EjectButton.isHover(pos)) {
        EjectButton.hover = false;
        outBuffer += "d_eject #" + std::to_string(idPlayer) + "\n";
        return true;
    }
    return false;
}

bool PlayerDataPlugin::onEvent(const irr::SEvent &event, pluginsData datas,
std::string &outBuffer) {
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
        irr::core::vector2d<irr::s32> pos =
            device->getCursorControl()->getPosition();
        LevelUpButton.isHover(pos);
        ForkButton.isHover(pos);
        EjectButton.isHover(pos);
        ForwardButton.isHover(pos);
        BackwardButton.isHover(pos);
        LeftButton.isHover(pos);
        RightButton.isHover(pos);

        if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
            pressed = true;
        } else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
            pressed = false;
        }
        if (pressed && !isPressedLastFrame) {
            if (handleClick(outBuffer, pos))
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
        std::shared_ptr<irr::scene::IAnimatedMeshSceneNode> playerMesh =
            player.PlayerMesh;
        if (!playerMesh || player.isDead)
            continue;
        irr::core::vector3df Pos = playerMesh->getPosition();
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
irr::video::IVideoDriver *driver) {
    if (!font || !driver)
        return;
    driver->draw2DRectangle(irr::video::SColor(
        blocked ? 100 : (hover ? 120 : 255), 87, 46, 47), rect);
    font->draw(name.c_str(), rect, irr::video::SColor(
        blocked ? 100 : 255, 255, 255, 255), true, true);
}

bool PlayerDataPlugin::Button::isHover(irr::core::vector2di pos) {
    hover = rect.isPointInside(pos);
    return hover;
}

void PlayerDataPlugin::Button::updatePos(
const irr::core::position2d<irr::s32> &pos) {
    rect = irr::core::rect<irr::s32>(pos.X, pos.Y,
        pos.X + rect.getWidth(), pos.Y + rect.getHeight());
}
