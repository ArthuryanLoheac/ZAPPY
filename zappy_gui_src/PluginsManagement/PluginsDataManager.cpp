#include "PluginsDataManager.hpp"

#include "include/logs.h"

void PluginsDataManager::updatePluginsData() {
    // Map
    data.height = GUI::GameDataManager::i().getHeight();
    data.width = GUI::GameDataManager::i().getWidth();
    // FREQ
    data.frequency = GUI::DataManager::i().getFrequency();
    // Teams
    data.teams = GUI::GameDataManager::i().getTeams();
    data.teamColors.clear();
    for (const auto &team : data.teams)
        data.teamColors.push_back(MeshImporter::i().getColor(team));
    // Tiles
    try {
        data.tiles.clear();
        for (int x = 0; x < data.width; ++x) {
            for (int y = 0; y < data.height; ++y) {
                GUI::GameTile &tile = GUI::GameDataManager::i().getTile(x, y);
                pluginsData::Tile newTile;
                newTile.x = x;
                newTile.y = y;
                for (int i = 0; i < 7; i++)
                    newTile.resources.push_back(tile.getRessource(i));
                data.tiles.push_back(newTile);
            }
        }
    } catch (std::exception &e) {
        LOG_ERROR("ERROR %s\n", e.what());
    }
}