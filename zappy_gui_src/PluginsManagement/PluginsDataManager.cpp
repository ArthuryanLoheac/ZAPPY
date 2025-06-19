#include "PluginsManagement/PluginsDataManager.hpp"
#include "Connection/ServerGUI.hpp"
#include "DataManager/GameDataManager.hpp"
#include "DataManager/DataManager.hpp"

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
    data.isConnected = GUI::ServerGUI::i().isConnectedToServer();
    data.ping = GUI::ServerGUI::i().ping;
    for (const auto &message : GUI::GameDataManager::i().getMessages())
        data.messages.push_back(message);
    for (const auto &team : data.teams)
        data.teamColors.push_back(MeshImporter::i().getColor(team));
    updatePlayers();
    updateTiles();
}

void PluginsDataManager::updatePlayers() {
    // Players
    data.players.clear();
    for (const auto &player : GUI::GameDataManager::i().getPlayers()) {
        pluginsData::Player newPlayer;
        newPlayer.id = player.getId();
        newPlayer.teamName = player.getTeamName();
        newPlayer.x = player.getX();
        newPlayer.y = player.getY();
        newPlayer.level = player.getLevel();
        newPlayer.color = MeshImporter::i().getColor(player.getTeamName());
        newPlayer.PlayerMesh = player.getMesh();
        newPlayer.position = irr::core::vector3df(
            player.getX(), 0, player.getY());
        switch (player.getOrientation()) {
            case GUI::Player::Orientation::NORTH:
                newPlayer.orientation = pluginsData::Player::Orientation::NORTH;
                break;
            case GUI::Player::Orientation::SOUTH:
                newPlayer.orientation = pluginsData::Player::Orientation::SOUTH;
                break;
            case GUI::Player::Orientation::EAST:
                newPlayer.orientation = pluginsData::Player::Orientation::EAST;
                break;
            case GUI::Player::Orientation::WEST:
                newPlayer.orientation = pluginsData::Player::Orientation::WEST;
                break;
        }
        newPlayer.ressources.clear();
        for (int i = 0; i < 7; ++i)
            newPlayer.ressources.push_back(player.getRessource(i));
        data.players.push_back(newPlayer);
    }
}

void PluginsDataManager::updateTiles() {
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
