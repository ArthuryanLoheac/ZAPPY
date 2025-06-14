#include "PluginsDataManager.hpp"

void PluginsDataManager::updatePluginsData() {
    data.height = GUI::GameDataManager::i().getHeight();
    data.width = GUI::GameDataManager::i().getWidth();
}