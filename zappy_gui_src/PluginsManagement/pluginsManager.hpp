#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "dlLoader/dlLoader.hpp"
#include "PluginsManagement/pluginsInterface.hpp"
#include "DataManager/GameDataManager.hpp"


class pluginsManager {
 public:
    static pluginsManager &i() {
        static pluginsManager instance;
        return instance;
    }

    /** @brief Load all plugins from the specified directory.
     * @param path The directory path where plugins are located.
     */
    void loadPlugins(const std::string &path);

    void initPlugins();

    /** @brief Load a single plugin from the specified path.
     * @param path The file path of the plugin to load.
     */
    void loadPlugin(const std::string &path);

    /** @brief Draw all loaded plugins' UI elements.
     * @param font Pointer to the GUI font used for rendering text.
     * @param driver Pointer to the video driver used for rendering graphics.
     */
    void drawPlugins(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver);

    /** @brief Handle events for all loaded plugins.
     * @param event The event to handle.
     */
    void onEvent(const irr::SEvent &event);

    /** @brief Update all plugins with the datas of the game
     * @param dataManager The data structure contaning all datas
     */
    void update(pluginsData dataManager);

    /**
     * @brief Sort the plugins based on their priority.
     * This function sorts the plugins in descending order of their priority.
     */
    void sortPlugins();

    /** @brief Initialize the plugins.
     * This function initializes all loaded plugins.
     */
    void initPluginMesh(initPluginData meshData);

    /** @brief Set the mesh data for a specific plugin.
     * @param key The key identifying the plugin.
     * @param meshData The mesh data to set for the plugin.
     */
    void setPluginMeshData(std::string key,
        initPluginData::MeshInitPlugin meshData);

    bool isWindowOpened() const {
        return windowOpened;
    }

    void saveActivePlugins();
 private:
    std::vector<std::unique_ptr<pluginsInterface>> _plugins;
        /**plugins stored */

    std::string pathSaveFile = "plugins/save.txt";

    // ========= WINDOW PAGE =========
    class ButtonPlugin {
     public:
        ButtonPlugin(int x, int y, int index)
            : x(x), y(y), index(index) {}
        int x;
        int y;
        int index;
    };

    bool windowOpened = false;
    int pluginIndex = 0;
    int showedPlugins = 6;

    std::vector<ButtonPlugin> buttons;

    std::unordered_map<std::string, irr::video::ITexture *> cachedTextures;
    /**< Cache for textures to avoid reloading. */

    void drawImage(const std::string &texture, int x, int y, int sizeX,
        int sizeY, irr::video::IVideoDriver* driver, int alpha);

    void onEventWindow(const irr::SEvent &event);

    void drawWindow(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver);

    void drawButton(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver, int i, int y);

    void loadActivePlugins();

};
