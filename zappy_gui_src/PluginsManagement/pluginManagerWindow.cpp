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


void pluginsManager::onEventWindow(const irr::SEvent &event)
{
    (void) event;
}

void pluginsManager::drawWindow(std::shared_ptr<irr::gui::IGUIFont> font,
    irr::video::IVideoDriver* driver) const
{
    (void) font;
    (void) driver;
}

void pluginsManager::updateWindow(pluginsData dataManager)
{
    (void) dataManager;
    printf("UPDATE WINDOW\n");
}
