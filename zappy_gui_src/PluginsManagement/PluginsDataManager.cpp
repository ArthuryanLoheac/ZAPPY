#include "PluginsDataManager.hpp"

void PluginsDataManager::updatePluginsData() {
    data.height = GUI::GameDataManager::i().getHeight();
    data.width = GUI::GameDataManager::i().getWidth();
    data.frequency = GUI::DataManager::i().getFrequency();
    data.teams = GUI::GameDataManager::i().getTeams();
    data.teamColors.clear();
    for (const auto &team : data.teams)
        data.teamColors.push_back(MeshImporter::i().getColor(team));
}