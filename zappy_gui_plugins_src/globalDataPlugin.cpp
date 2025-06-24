#include "zappy_gui_plugins_src/globalDataPlugin.hpp"

#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<globalDataPlugin>();
    }
}

void globalDataPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
irr::video::IVideoDriver* driver) {
    int x = 30;
    int y = 30;
    int spaceBetween = 30;
    UICol white(255, 255, 255, 255);

    if (!font)
        return;
    int sizeY = 120 + data.teams.size() * 30;
    for (size_t i = 0; i < data.teams.size(); i++)
        sizeY += 20 * countNbLevelDifferentTeams(data.teams[i]);
    drawImage("assets/UI/BottomRight.png", 0, 0, 200, sizeY, driver);
    // FPS
    font->draw(("FPS : " + std::to_string(driver->getFPS())).c_str(),
        UIRect(x, y, 300, 50), white);
    // MAP
    y += spaceBetween;
    font->draw(("MAP (" + std::to_string(data.width) + "x" +
        std::to_string(data.height) + ")").c_str(),
        UIRect(x, y, 300, 50), white);
    // TEAMS
    y += spaceBetween;
    font->draw(("TEAMS (" + std::to_string(data.players.size()) + ") :")
        .c_str(), UIRect(x, y, 300, 50), white);
    int i = 0;
    for (auto &team : data.teams) {
        y += 20;
        std::string teamStr = "\t" + team + " (" +
            std::to_string(countNbPlayersOnTeam(team)) + ")";
        font->draw(teamStr.c_str(), UIRect(x, y, 300, 50),
            data.teamColors[i % data.teamColors.size()]);
        for (int level = 1; level <= 8; level++) {
            int count = countNbPlayersOnTeam(team, level);
            if (count > 0) {
                y+= 20;
                std::string levelStr = "\t\tLevel " + std::to_string(level) +
                    " : " + std::to_string(count);
                font->draw(levelStr.c_str(), UIRect(x + 20, y, 300, 50),
                    data.teamColors[i % data.teamColors.size()]);
            }
        }
        i++;
    }
}

int globalDataPlugin::countNbPlayersOnTeam(const std::string &teamName) {
    int count = 0;
    for (const auto &player : data.players) {
        if (player.teamName == teamName) {
            count++;
        }
    }
    return count;
}

int globalDataPlugin::countNbPlayersOnTeam(const std::string &teamName,
int level) {
    int count = 0;
    for (const auto &player : data.players) {
        if (player.teamName == teamName && player.level == level) {
            count++;
        }
    }
    return count;
}

int globalDataPlugin::countNbLevelDifferentTeams(const std::string &teamName) {
    std::vector<int> levels;

    for (const auto &player : data.players) {
        if (player.teamName == teamName) {
            if (std::find(levels.begin(), levels.end(), player.level) ==
                levels.end()) {
                levels.push_back(player.level);
            }
        }
    }
    return levels.size();
}