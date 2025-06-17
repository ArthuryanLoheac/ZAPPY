#include <dlfcn.h>
#include <dirent.h>
#include <irrlicht/irrlicht.h>

#include <iostream>
#include <memory>
#include <string>
#include <utility>

#include "include/logs.h"
#include "PluginsManagement/pluginsManager.hpp"
#include "DataManager/DataManager.hpp"
#include "Graphic/Window/window.hpp"
#include "Connection/ServerGUI.hpp"
#include "PluginsDataManager.hpp"


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
    if (!dlLoader<pluginsInterface>::verifyLib(path, "createPlugin")) {
        std::cerr << "Failed to load plugin from: " << path << std::endl;
        return;
    }
    try {
        auto plugin = dlLoader<pluginsInterface>::getLib(path, "createPlugin");
        if (plugin) {
            plugin->init(GUI::Window::i().smgr, GUI::Window::i().device,
                GUI::Window::i().cam);
            _plugins.push_back(std::move(plugin));
        }
    } catch (const dlLoader<pluginsInterface>::dlError &e) {
        std::cerr << e.what() << std::endl;
    }
}

void pluginsManager::drawPlugins(std::shared_ptr<irr::gui::IGUIFont> font,
    irr::video::IVideoDriver* driver) const {
    for (const auto &plugin : _plugins) {
        if (plugin)
            plugin->drawUI(font, driver);
    }
}

void pluginsManager::onEvent(const irr::SEvent &event) {
    for (const auto &plugin : _plugins) {
        if (plugin) {
            pluginsData &datas = PluginsDataManager::i().getData();
            bool newData = plugin->onEvent(event, datas);
            if (datas.frequency > 0 &&
                datas.frequency != GUI::DataManager::i().getFrequency()) {
                GUI::DataManager::i().setFrequency(datas.frequency);
                GUI::ServerGUI::i().outbuffer += "sst " +
                    std::to_string(datas.frequency) + "\n";
                PluginsDataManager::i().updatePluginsData();
            }
            if (newData)
                return;
        }
    }
}

void pluginsManager::update(pluginsData dataManager) {
    for (const auto &plugin : _plugins) {
        if (plugin)
            plugin->update(dataManager);
    }
}
