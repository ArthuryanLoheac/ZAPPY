#include <dlfcn.h>
#include <dirent.h>
#include <irrlicht/irrlicht.h>

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "include/logs.h"
#include "pluginsManager.hpp"

void pluginsManager::loadPlugins(const std::string &path) {
    DIR *dir = opendir(path.c_str());
    struct dirent *entry;

    if (dir == nullptr) {
        LOG_ERROR("Failed to open directory: %s", path.c_str());
        return;
    }
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_REG)
            loadPlugin(path + "/" + entry->d_name);
    }
    closedir(dir);
}

void pluginsManager::loadPlugin(const std::string &path) {
    if (dlLoader<pluginsInterface>::verifyLib(path, "createPlugin")) {
        try {
            auto plugin = dlLoader<pluginsInterface>::getLib(path, "createPlugin");
            if (plugin) {
                plugin->init();
                _plugins.push_back(std::move(plugin));
            }
        } catch (const dlLoader<pluginsInterface>::dlError &e) {
            std::cerr << e.what() << std::endl;
        }
    } else {
        std::cerr << "Failed to load plugin from: " << path << std::endl;
    }
}

void pluginsManager::drawPlugins(std::shared_ptr<irr::gui::IGUIFont> font) const
{
    for (const auto &plugin : _plugins) {
        if (plugin)
            plugin->drawUI(font);
    }
}
