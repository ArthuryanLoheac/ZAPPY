#include <dlfcn.h>
#include <dirent.h>
#include <irrlicht/irrlicht.h>

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <algorithm>

#include "include/logs.h"
#include "PluginsManagement/pluginsManager.hpp"
#include "DataManager/DataManager.hpp"
#include "Graphic/Window/window.hpp"
#include "Connection/ServerGUI.hpp"
#include "PluginsManagement/PluginsDataManager.hpp"
#include "DataManager/PathManager.hpp"


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
        LOG_ERROR("Plugin %s is not a valid plugin", path.c_str());
        return;
    }
    try {
        auto plugin = dlLoader<pluginsInterface>::getLib(path, "createPlugin");
        if (plugin) {
            initPluginData data = plugin->init(GUI::Window::i().smgr,
                GUI::Window::i().device, GUI::Window::i().cam);
            initPluginMesh(data);
            _plugins.push_back(std::move(plugin));
        }
    } catch (const dlLoader<pluginsInterface>::dlError &e) {
        std::cerr << e.what() << std::endl;
    }
}

void pluginsManager::drawPlugins(std::shared_ptr<irr::gui::IGUIFont> font,
    irr::video::IVideoDriver* driver) {
    if (windowOpened)
        drawWindow(font, driver);

    for (const auto &plugin : _plugins) {
        if (plugin && plugin->isActive()) {
            try {
                plugin->drawUI(font, driver);
            } catch (const std::exception &e) {
                LOG_ERROR("Error while drawing plugin: %s", e.what());
            }
        }
    }
}

void pluginsManager::onEvent(const irr::SEvent &event) {
    if (event.EventType == irr::EET_KEY_INPUT_EVENT &&
        event.KeyInput.Key == irr::KEY_TAB &&
        event.KeyInput.PressedDown)
        windowOpened = !windowOpened;
    if (windowOpened) {
        onEventWindow(event);
        return;
    }
    for (const auto &plugin : _plugins) {
        if (plugin && plugin->isActive()) {
            try {
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
            } catch (const std::exception &e) {
                LOG_ERROR("Error while processing event in plugin: %s",
                    e.what());
            }
        }
    }
}

void pluginsManager::update(pluginsData dataManager) {
    for (const auto &plugin : _plugins) {
        if (plugin && plugin->isActive())
            plugin->update(dataManager);
    }
}

void pluginsManager::sortPlugins() {
    std::sort(_plugins.begin(), _plugins.end(),
        [](const std::unique_ptr<pluginsInterface> &a,
           const std::unique_ptr<pluginsInterface> &b) {
            return a->getPriority() > b->getPriority();
        });
}

void pluginsManager::initPluginMesh(initPluginData meshData) {
    setPluginMeshData("Food", meshData.MeshBattery);
    setPluginMeshData("Mat1", meshData.MeshMat1);
    setPluginMeshData("Mat2", meshData.MeshMat2);
    setPluginMeshData("Mat3", meshData.MeshMat3);
    setPluginMeshData("Mat4", meshData.MeshMat4);
    setPluginMeshData("Mat5", meshData.MeshMat5);
    setPluginMeshData("Mat6", meshData.MeshMat6);
    setPluginMeshData("Player", meshData.MeshPlayer);
    setPluginMeshData("Egg", meshData.MeshEgg);
    setPluginMeshData("Tile", meshData.MeshTile);
    if (meshData.skyBox.isSet) {
        GUI::PathManager::i().setPath("skyboxTop", meshData.skyBox.top);
        GUI::PathManager::i().setPath("skyboxBottom", meshData.skyBox.bottom);
        GUI::PathManager::i().setPath("skyboxLeft", meshData.skyBox.left);
        GUI::PathManager::i().setPath("skyboxRight", meshData.skyBox.right);
        GUI::PathManager::i().setPath("skyboxFront", meshData.skyBox.front);
        GUI::PathManager::i().setPath("skyboxBack", meshData.skyBox.back);
        GUI::Window::i().setRotationSpeedSkybox(meshData.skyBox.speedRotation);
    }
}

void pluginsManager::setPluginMeshData(std::string key,
initPluginData::MeshInitPlugin meshData) {
    if (meshData.isSet) {
       GUI::PathManager::i().setPath(key, meshData.name);
       GUI::PathManager::i().setScale(meshData.name, meshData.scale);
    }
}
