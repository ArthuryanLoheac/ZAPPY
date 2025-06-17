#include <irrlicht/irrlicht.h>

#include <iostream>

#include "Exceptions/GraphicalExceptions.hpp"
#include "Graphic/Window/window.hpp"
#include "DataManager/DataManager.hpp"
#include "PluginsManagement/pluginsManager.hpp"
#include "DataManager/SoundsManager.hpp"

/**
 * @brief Initializes and runs the graphical interface.
 *
 * @return int Returns 0 on success, 84 on failure.
 */
int graphic() {
    try {
        GUI::Window::i();
        pluginsManager::i().loadPlugins("./plugins");
        pluginsManager::i().sortPlugins();
        GUI::SoundsManager::i().playMusic("assets/Musics/Ambient.mp3");
        GUI::Window::i().update();
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 84;
    }
    GUI::DataManager::i().setRunning(false);
    return 0;
}
