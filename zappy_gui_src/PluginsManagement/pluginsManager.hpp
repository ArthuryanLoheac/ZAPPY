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

    /** @brief Initialize all loaded plugins.
     * This function initializes each plugin and prepares them for use.
     */
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

    /** @brief Set the sound data for a specific plugin.
     * @param key The key identifying the plugin.
     * @param soundData The sound data to set for the plugin.
     */
    void setPluginSoundData(std::string key,
        initPluginData::SoundInitPlugin soundData);

    /** @brief Check if the plugins management window is opened.
     * @return True if the window is opened, false otherwise.
     */
    bool isWindowOpened() const {
        return windowOpened;
    }

    /** @brief Save the state of active plugins to a file.
     * This function writes the names and active states of all plugins
     */
    void saveActivePlugins();

 private:
    std::vector<std::unique_ptr<pluginsInterface>> _plugins;
        /**plugins stored */

    std::string pathSaveFile = "plugins/save.txt";
        /**< Path to the save file for active plugins. */

    // ========= WINDOW PAGE =========
    class ButtonPlugin {
    /** @brief Represents a button for a plugin in the management window.
     * This class holds the position and index of the plugin associated with the button.
     */
     public:
        ButtonPlugin(int x, int y, int index)
            : x(x), y(y), index(index) {}
        int x; /**< X-coordinate of the button. */
        int y; /**< Y-coordinate of the button. */
        int index; /**< Index of the plugin associated with the button. */
    };

    bool windowOpened = false;
        /**< Flag to indicate if the plugins management window is open. */
    int pluginIndex = 0;
        /**< Index of the first plugin displayed in the window. */
    int showedPlugins = 6; /**< Number of plugins displayed in the window. */

    std::vector<ButtonPlugin> buttons;
        /**< List of buttons for each plugin in the window. */

    std::unordered_map<std::string, irr::video::ITexture *> cachedTextures;
    /**< Cache for textures to avoid reloading. */

    /** @brief Draw an image on the screen.
     * This function draws an image at the specified position with the given size
     * and alpha transparency.
     * @param texture The name of the texture to draw.
     * @param x The x-coordinate where the image will be drawn.
     * @param y The y-coordinate where the image will be drawn.
     * @param sizeX The width of the image.
     * @param sizeY The height of the image.
     * @param driver Pointer to the video driver used for rendering graphics.
     * @param alpha The alpha transparency value (0-255).
     */
    void drawImage(const std::string &texture, int x, int y, int sizeX,
        int sizeY, irr::video::IVideoDriver* driver, int alpha);

    /** @brief Handle events for the plugins management window.
     * This function processes events such as mouse clicks and key presses
     * to manage the plugins in the window.
     * @param event The event to handle.
     */
    void onEventWindow(const irr::SEvent &event);

    /** @brief Draw the plugins management window.
     * This function draws the window containing the list of plugins and their
     * activation buttons.
     * @param font Pointer to the GUI font used for rendering text.
     * @param driver Pointer to the video driver used for rendering graphics.
     */
    void drawWindow(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver);

    /** @brief Draw a button for each plugin in the window.
     * @param font Pointer to the GUI font used for rendering text.
     * @param driver Pointer to the video driver used for rendering graphics.
     * @param i The index of the plugin.
     * @param y The vertical position where the button will be drawn.
     */
    void drawButton(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver, int i, int y);

    /** @brief Load if a plugins is active from the save file.
     * This function reads the save file and activates the plugins accordingly.
     */
    void loadActivePlugins();
};
