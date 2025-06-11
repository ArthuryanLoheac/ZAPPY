#include <iostream>
#include <memory>

#include "Graphic/Window/window.hpp"
#include "Graphic/Events/MyEventReceiver.hpp"
#include "tools/MeshImporter.hpp"
#include "DataManager/DataManager.hpp"
#include "window.hpp"


namespace GUI {
void Window::drawUI() {
    int x = 300;
    int y = 10;

    if (!font)
        return;
    // TEAMS
    font->draw("TEAMS : ", UIRect(x, y, 300, 500), UICol(255, 255, 255, 255));
    for (auto &team : GUI::GameDataManager::i().getTeams()) {
        y += 20;
        Vec3d pos(x, y, 0);
        font->draw(("\t" + team).c_str(), UIRect(pos.X, pos.Y, 300, 50),
            MeshImporter::i().getColor(team));
    }

    // FPS
    font->draw(("FPS : " + std::to_string(driver->getFPS())).c_str(),
        UIRect(10, 10, 300, 50), UICol(255, 255, 255, 255));

    // Frequency
    font->draw(("Freq : " +
        std::to_string(GUI::DataManager::i().getFrequency())).c_str(),
        UIRect(150, 10, 300, 50), UICol(255, 255, 255, 255));

    // Tile
    if (xTile != -1 && yTile != -1)
        drawTileInfo(GUI::GameDataManager::i().getTile(xTile, yTile));

    if (idPlayer != -1)
        drawPlayerInfo(idPlayer);
}

void Window::drawTileInfo(GameTile &tile)
{
    int width = driver->getScreenSize().Width;
    std::vector<std::string> lstNames = {
        "Food", "Linemate", "Deraumere", "Sibur", "Mendiane", "Phiras", "Thystame"
    };
    std::vector<UICol> lstColors = {
        UICol(255, 0, 0, 0), UICol(255, 200, 193, 198),
        UICol(255, 55, 55, 55), UICol(255, 71, 73, 116),
        UICol(255, 94, 84, 33), UICol(255, 94, 31, 32),
        UICol(255, 64, 35, 94)
    };


    std::string tileInfo = "Tile : " + std::to_string(tile.getX()) +
        ", " + std::to_string(tile.getY()) + " :";
    font->draw(tileInfo.c_str(), UIRect(width - 200, 10, 300, 300),
        UICol(255, 0, 0, 0));
    int y = 30;
    for (int i = 0; i < 7; ++i) {
        if (tile.getRessource(i) == 0)
            continue;
        tileInfo = "\t - " + lstNames[i] + " : " +
            std::to_string(tile.getRessource(i));
        font->draw(tileInfo.c_str(), UIRect(1100, y, 300, 300),
            lstColors[i]);
        y += 20;
    }
}

void Window::drawPlayerInfo(int id)
{
    int width = driver->getScreenSize().Width;
    Player &player = GUI::GameDataManager::i().getPlayer(id);

    // Name
    std::string playerInfo = "Player " + std::to_string(id) + " : " +
        player.getTeamName();
    font->draw(playerInfo.c_str(), UIRect(width - 200, 110, 300, 300),
        MeshImporter::i().getColor(player.getTeamName()));
    // Level
    playerInfo = "\tLevel : " + std::to_string(player.getLevel());
    font->draw(playerInfo.c_str(), UIRect(width - 200, 130, 300, 300),
        UICol(255, 0, 0, 0));
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
    font->draw(playerInfo.c_str(), UIRect(width - 200, 150, 300, 300),
        UICol(255, 0, 0, 0));
    // Inventory
    int y = 170;
    std::vector<std::string> lstNames = {
        "Food", "Linemate", "Deraumere", "Sibur", "Mendiane", "Phiras", "Thystame"
    };
    std::vector<UICol> lstColors = {
        UICol(255, 0, 0, 0), UICol(255, 200, 193, 198),
        UICol(255, 55, 55, 55), UICol(255, 71, 73, 116),
        UICol(255, 94, 84, 33), UICol(255, 94, 31, 32),
        UICol(255, 64, 35, 94)
    };
    for (int i = 0; i < 7; ++i) {
        if (player.getRessource(i) == 0)
            continue;
        playerInfo = "\t - " + lstNames[i] + " : " +
            std::to_string(player.getRessource(i));
        font->draw(playerInfo.c_str(), UIRect(width - 200, y, 300, 300),
            lstColors[i]);
        y += 20;
    }
}
}  // namespace GUI
