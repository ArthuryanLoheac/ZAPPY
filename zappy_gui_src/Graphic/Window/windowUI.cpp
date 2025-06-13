#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Graphic/Window/window.hpp"
#include "Graphic/Events/MyEventReceiver.hpp"
#include "tools/MeshImporter.hpp"
#include "DataManager/DataManager.hpp"


namespace GUI {
void Window::drawOneBackground(const std::string &texture, int x, int y,
int sizeX, int sizeY) {
    irr::video::ITexture* bg = driver->getTexture(texture.c_str());
    irr::core::rect<irr::s32> sourceRect(0, 0, 1000, 1000);

    irr::core::rect<irr::s32>destRect(x, y, x + sizeX, y + sizeY);
    if (!bg) {
        std::cerr << "Error: Texture not found: " << texture << std::endl;
        return;
    }
    driver->draw2DImage(bg, destRect, sourceRect, 0, nullptr, true);
}

void Window::drawBackgrounds() {
    int width = driver->getScreenSize().Width;

    // Draw the left bar
    drawOneBackground("assets/UI/BottomRight.png", 0, 0, 150, 400);
    // Draw the Right bar
    if (idPlayer != -1 && xTile != -1 && yTile != -1)
        drawOneBackground("assets/UI/BottomLeft.png", width - 240, 0, 240, 400);
    else if (idPlayer != -1)
        drawOneBackground("assets/UI/BottomLeft.png", width - 240, 0, 240, 200);
    else if (xTile != -1 && yTile != -1)
        drawOneBackground("assets/UI/BottomLeft.png", width - 240, 0, 240, 200);
}

void Window::drawUI() {
    int x = 30;
    int y = 30;
    int spaceBetween = 30;

    drawBackgrounds();
    if (!font)
        return;
    // FPS
    font->draw(("FPS : " + std::to_string(driver->getFPS())).c_str(),
        UIRect(x, y, 300, 50), UICol(255, 255, 255, 255));
    // Frequency
    y += spaceBetween;
    font->draw(("Freq : " +
        std::to_string(GUI::DataManager::i().getFrequency())).c_str(),
        UIRect(x, y, 300, 50), UICol(255, 255, 255, 255));
    // TEAMS
    y += spaceBetween;
    font->draw("TEAMS : ", UIRect(x, y, 300, 50), UICol(255, 255, 255, 255));
    for (auto &team : GUI::GameDataManager::i().getTeams()) {
        y += 20;
        font->draw(("\t" + team).c_str(), UIRect(x, y, 300, 50),
            MeshImporter::i().getColor(team));
    }

    int yR = 20;
    // Tile
    if (xTile != -1 && yTile != -1)
        drawTileInfo(GUI::GameDataManager::i().getTile(xTile, yTile), yR);

    try {
        if (idPlayer != -1)
            drawPlayerInfo(idPlayer, yR);
    } catch (const std::exception &e) {
        idPlayer = -1;
    }
}

void Window::drawTileInfo(GameTile &tile, int &y) {
    int width = driver->getScreenSize().Width;
    std::vector<std::string> lstNames = {"Food", "Linemate", "Deraumere",
        "Sibur", "Mendiane", "Phiras", "Thystame"};
    std::vector<UICol> lstColors = {
        UICol(255, 255, 255, 255), UICol(255, 200, 193, 198),
        UICol(255, 55, 55, 55), UICol(255, 71, 73, 116),
        UICol(255, 94, 84, 33), UICol(255, 94, 31, 32),
        UICol(255, 64, 35, 94)
    };


    std::string tileInfo = "Tile : " + std::to_string(tile.getX()) +
        ", " + std::to_string(tile.getY()) + " :";
    font->draw(tileInfo.c_str(), UIRect(width - 220, y, 300, 300),
        UICol(255, 255, 255, 255));
    y += 20;
    for (int i = 0; i < 7; ++i) {
        if (tile.getRessource(i) == 0)
            continue;
        tileInfo = "\t - " + lstNames[i] + " : " +
            std::to_string(tile.getRessource(i));
        font->draw(tileInfo.c_str(), UIRect(width - 220, y, 300, 300),
            lstColors[i]);
        y += 20;
    }
    y += 20;
}

void Window::drawPlayerInfo(int id, int &y) {
    int width = driver->getScreenSize().Width;
    Player &player = GUI::GameDataManager::i().getPlayer(id);

    // Name
    std::string playerInfo = "Player " + std::to_string(id) + " : " +
        player.getTeamName();
    font->draw(playerInfo.c_str(), UIRect(width - 220, y, 300, 300),
        MeshImporter::i().getColor(player.getTeamName()));
    // Level
    y += 20;
    playerInfo = "\tLevel : " + std::to_string(player.getLevel());
    font->draw(playerInfo.c_str(), UIRect(width - 220, y, 300, 300),
        UICol(255, 255, 255, 255));
    // Position
    playerInfo = "\tPos : " + std::to_string(player.getX()) +
        ", " + std::to_string(player.getY()) + " - ";
    // Direction
    switch (player.getOrientation()) {
        case Player::Orientation::NORTH:
            playerInfo += "North";
            break;
        case Player::Orientation::SOUTH:
            playerInfo += "South";
            break;
        case Player::Orientation::EAST:
            playerInfo += "East";
            break;
        case Player::Orientation::WEST:
            playerInfo += "West";
            break;
    }
    y += 20;
    font->draw(playerInfo.c_str(), UIRect(width - 220, y, 300, 300),
        UICol(255, 255, 255, 255));
    // Inventory
    y += 20;
    std::vector<std::string> lstNames = {"Food", "Linemate", "Deraumere",
        "Sibur", "Mendiane", "Phiras", "Thystame"};
    std::vector<UICol> lstColors = {
        UICol(255, 255, 255, 255), UICol(255, 200, 193, 198),
        UICol(255, 55, 55, 55), UICol(255, 71, 73, 116),
        UICol(255, 94, 84, 33), UICol(255, 94, 31, 32),
        UICol(255, 64, 35, 94)};
    for (int i = 0; i < 7; ++i) {
        if (player.getRessource(i) == 0)
            continue;
        playerInfo = "\t - " + lstNames[i] + " : " +
            std::to_string(player.getRessource(i));
        font->draw(playerInfo.c_str(), UIRect(width - 220, y, 300, 300),
            lstColors[i]);
        y += 20;
    }
}
}  // namespace GUI
