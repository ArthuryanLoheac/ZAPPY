#pragma once
#include "PluginsManagement/pluginsData.hpp"
#include "DataManager.hpp"
#include "GameDataManager.hpp"

class PluginsDataManager
{
 private:
    pluginsData data;
 public:
    static PluginsDataManager &i() {
        static PluginsDataManager instance;
        return instance;
    }

    void updatePluginsData();
    pluginsData &getData() {
        return data;
    }
};
