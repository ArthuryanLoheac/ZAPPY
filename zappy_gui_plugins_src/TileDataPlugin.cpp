#include "zappy_gui_plugins_src/TileDataPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<TileDataPlugin>();
    }
}

bool TileDataPlugin::init(irr::scene::ISceneManager* _smgr,
    irr::IrrlichtDevice *_device, irr::scene::ICameraSceneNode *_cam) {
    device = _device;
    smgr = _smgr;
    cam = _cam;
    printf("============= Initializing TileData Plugin =============\n");
    return true;
}

void TileDataPlugin::drawOneBackground(const std::string &texture, int x, int y,
int sizeX, int sizeY, irr::video::IVideoDriver* driver) {
    irr::video::ITexture* bg = driver->getTexture(texture.c_str());
    irr::core::rect<irr::s32> sourceRect(0, 0, 1000, 1000);

    irr::core::rect<irr::s32>destRect(x, y, x + sizeX, y + sizeY);
    if (!bg) {
        std::cerr << "Error: Texture not found: " << texture << std::endl;
        return;
    }
    driver->draw2DImage(bg, destRect, sourceRect, 0, nullptr, true);
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

    if (data.tiles.size() <= 0 || (xTile == -1 && yTile == -1))
        return;
    try {
        pluginsData::Tile tile = getTile(xTile, yTile);
        drawOneBackground("assets/UI/BottomLeft.png", width - 240, 0, 240, 200,
            driver);

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
        y += 20;
    } catch (std::exception &e) {}
}

bool TileDataPlugin::onEvent(const irr::SEvent &event, pluginsData &datas) {
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
        if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
            pressed = true;
        } else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
            pressed = false;
        }
        if (pressed && !isPressedLastFrame && !detectCollisionPlayer())
            detectCollisionTile();
        isPressedLastFrame = pressed;
    }
    (void) datas;
    return false;
}

void TileDataPlugin::detectCollisionTile() {
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
            return;
        if (xTile == x && yTile == y) {
            xTile = -1;
            yTile = -1;
            return;
        }
        xTile = x;
        yTile = y;
    }
}

bool TileDataPlugin::detectCollisionPlayer() {
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
        if (box.intersectsWithLine(ray))
            return true;
    }
    return false;
}

void TileDataPlugin::update(pluginsData _data) {
    data = _data;
}

int TileDataPlugin::getPriority() const {
    return 0;
}
