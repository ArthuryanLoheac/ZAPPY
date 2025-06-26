#include "zappy_gui_plugins_src/TileDataPluginDebug.hpp"

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <map>
#include <algorithm>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<TileDataPluginDebug>();
    }
}

pluginsData::Tile TileDataPluginDebug::getTile(int x, int y) {
    for (auto tile : data.tiles) {
        if (tile.x == x && tile.y == y)
            return tile;
    }
    throw std::out_of_range("Not Exsiting");
}

void TileDataPluginDebug::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver* driver) {
    int width = driver->getScreenSize().Width;
    int y = 20;

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

        std::string tileInfo = "Tile : " + std::to_string(tile.x) + ", " +
            std::to_string(tile.y) + " :";
        font->draw(tileInfo.c_str(), UIRect(width - 220, y, 300, 300),
            UICol(255, 255, 255, 255));
        y += 30;

        drawDoubleButtons(FoodMinusButton, FoodPlusButton, font, driver,
            0, tile, y);
        drawDoubleButtons(LinemateMinusButton, LinematePlusButton, font,
            driver, 1, tile, y);
        drawDoubleButtons(DeraumereMinusButton, DeraumerePlusButton, font,
            driver, 2, tile, y);
        drawDoubleButtons(SiburMinusButton, SiburPlusButton, font, driver,
            3, tile, y);
        drawDoubleButtons(MendianeMinusButton, MendianePlusButton, font,
            driver, 4, tile, y);
        drawDoubleButtons(PhirasMinusButton, PhirasPlusButton, font, driver,
            5, tile, y);
        drawDoubleButtons(ThystameMinusButton, ThystamePlusButton, font,
            driver, 6, tile, y);
    } catch (std::exception &e) {
    }
}

void TileDataPluginDebug::drawDoubleButtons(TileDataPluginDebug::Button &bMinus,
TileDataPluginDebug::Button &bPlus, std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver *driver, int ressource, pluginsData::Tile tile,
int &y) {
    int width = driver->getScreenSize().Width;

    bMinus.updatePos({width - 220, y});
    font->draw(std::to_string(tile.resources[ressource]).c_str(),
        UIRect(width - 120, y + 5, 300, 300), UICol(255, 255, 255, 255));
    bPlus.updatePos({width - 80, y});
    bPlus.draw(font, driver);
    bMinus.draw(font, driver);
    y += 34;
}

bool TileDataPluginDebug::detectCollisionTile() {
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

bool TileDataPluginDebug::handleClick(std::string &outBuffer,
irr::core::vector2d<irr::s32> pos) {
    if (detectCollisionTile())
        return true;
    if (updateDoubleButton("food", FoodMinusButton,
        FoodPlusButton, outBuffer, pos))
        return true;
    if (updateDoubleButton("linemate", LinemateMinusButton,
        LinematePlusButton, outBuffer, pos))
        return true;
    if (updateDoubleButton("deraumere", DeraumereMinusButton,
        DeraumerePlusButton, outBuffer, pos))
        return true;
    if (updateDoubleButton("sibur", SiburMinusButton,
        SiburPlusButton, outBuffer, pos))
        return true;
    if (updateDoubleButton("mendiane", MendianeMinusButton,
        MendianePlusButton, outBuffer, pos))
        return true;
    if (updateDoubleButton("phiras", PhirasMinusButton,
        PhirasPlusButton, outBuffer, pos))
        return true;
    if (updateDoubleButton("thystame", ThystameMinusButton,
        ThystamePlusButton, outBuffer, pos))
        return true;
    return false;
}

bool TileDataPluginDebug::updateDoubleButton(std::string ressourceName,
TileDataPluginDebug::Button &bMinus, TileDataPluginDebug::Button &bPlus,
std::string &outBuffer, irr::core::vector2d<irr::s32> pos) {
    if (bPlus.isHover(pos)) {
        bPlus.hover = false;
        outBuffer += "D_ressource " + std::to_string(xTile) + " " +
            std::to_string(yTile) + " " + ressourceName + " 1\n";
        return true;
    }
    if (bMinus.isHover(pos)) {
        bMinus.hover = false;
        outBuffer += "D_ressource " + std::to_string(xTile) + " " +
            std::to_string(yTile) + " " + ressourceName + " -1\n";
        return true;
    }
    return false;
}


bool TileDataPluginDebug::onEvent(const irr::SEvent &event, pluginsData datas,
                                  std::string &outBuffer) {
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
        irr::core::vector2d<irr::s32> pos =
            device->getCursorControl()->getPosition();
        FoodPlusButton.isHover(pos);
        FoodMinusButton.isHover(pos);
        LinematePlusButton.isHover(pos);
        LinemateMinusButton.isHover(pos);
        DeraumerePlusButton.isHover(pos);
        DeraumereMinusButton.isHover(pos);
        SiburPlusButton.isHover(pos);
        SiburMinusButton.isHover(pos);
        MendianePlusButton.isHover(pos);
        MendianeMinusButton.isHover(pos);
        PhirasPlusButton.isHover(pos);
        PhirasMinusButton.isHover(pos);
        ThystamePlusButton.isHover(pos);
        ThystameMinusButton.isHover(pos);

        if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
            pressed = true;
        } else if (event.MouseInput.Event == irr::EMIE_LMOUSE_LEFT_UP) {
            pressed = false;
        }
        if (pressed && !isPressedLastFrame)
            return handleClick(outBuffer, pos);
        isPressedLastFrame = pressed;
    }
    (void) datas;
    return false;
}

// ============ BUTTON ============

void TileDataPluginDebug::Button::draw(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver *driver) {
    if (!font || !driver)
        return;
    driver->draw2DRectangle(irr::video::SColor(
        blocked ? 100 : (hover ? 120 : 255), 87, 46, 47), rect);
    font->draw(name.c_str(), rect, irr::video::SColor(
        blocked ? 100 : 255, 255, 255, 255), true, true);
}

bool TileDataPluginDebug::Button::isHover(irr::core::vector2di pos) {
    hover = rect.isPointInside(pos);
    return hover;
}

void TileDataPluginDebug::Button::updatePos(
const irr::core::position2d<irr::s32> &pos) {
    rect = irr::core::rect<irr::s32>(pos.X, pos.Y,
        pos.X + rect.getWidth(), pos.Y + rect.getHeight());
}
