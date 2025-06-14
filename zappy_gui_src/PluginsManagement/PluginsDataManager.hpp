#pragma once
#include "PluginsManagement/pluginsData.hpp"
#include "DataManager.hpp"
#include "GameDataManager.hpp"
#include "MeshImporter.hpp"

class PluginsDataManager
{
 /** @brief Singleton class that manages the plugins data for the game. */
 private:
    pluginsData data;
 public:
    /** @brief Singleton instance of PluginsDataManager. */
    static PluginsDataManager &i() {
        static PluginsDataManager instance;
        return instance;
    }

    /** @brief Updates the plugins data with the current game state. */
    void updatePluginsData();
    /** @brief Updates the players data in the plugins data. */
    void updatePlayers();
    /** @brief Updates the tiles data in the plugins data. */
    void updateTiles();

    /** @brief Retrieves the current plugins data.
     * @return Reference to the pluginsData object containing the game state.
     */
    pluginsData &getData() { return data; }
};
