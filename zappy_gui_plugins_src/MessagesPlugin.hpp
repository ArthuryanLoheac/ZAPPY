#pragma once
#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/pluginsInterface.hpp"

/**
 * @class MessagesPlugin
 * @brief Plugin class for managing global data visualization and interaction.
 * @implements pluginsInterface
 */
class MessagesPlugin : public pluginsInterface {
 private:
    class Particle {
      public:
        irr::scene::IParticleSystemSceneNode* particleSystem;
          /**< Particle system node. */
        irr::scene::IParticleEmitter* emitter; /**< Particle emitter. */
        irr::u32 end; /**< Timer for particle system end time. */
        irr::u32 kill; /**< Timer for particle system end time. */
        bool stopped = false;
    };

    pluginsData data; /**< Data manager for the plugin. */
    bool isActive = true; /**< Indicates if the plugin is currently active. */
    irr::scene::ISceneManager* smgr;
    irr::IrrlichtDevice *device;
    int speedParticle = 1;
    std::vector<Particle> particles;

    /**
     * @brief Draws a background texture at a specified position and size.
     * @param texture The texture file path.
     * @param x The X position.
     * @param y The Y position.
     * @param sizeX The width of the background.
     * @param sizeY The height of the background.
     * @param driver Pointer to the video driver.
     */
    void drawImage(const std::string &texture, int x, int y,
        int sizeX, int sizeY, irr::video::IVideoDriver* driver);
    void drawMessageHistory(std::shared_ptr<irr::gui::IGUIFont> font,
         irr::video::IVideoDriver* driver);
    void initParticle(irr::video::IVideoDriver* driver,
        irr::core::vector3df position, irr::core::vector3df direction, int age);
    void checkDeleteParticles();
    void SendParticlesToAll(irr::video::IVideoDriver* driver,
        pluginsData::Player p);

 public:
    /**
     * @brief Initializes the plugin with the necessary Irrlicht components.
     * @param smgr Pointer to the scene manager.
     * @param device Pointer to the Irrlicht device.
     * @param cam Pointer to the camera scene node.
     * @return True if initialization is successful, false otherwise.
     */
    bool init(irr::scene::ISceneManager* smgr,
    irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) override;

    /**
     * @brief Draws the UI elements for the plugin.
     * @param font Shared pointer to the GUI font.
     * @param driver Pointer to the video driver.
     */
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;

    /**
     * @brief Handles events for the plugin.
     * @param event The event to handle.
     */
    bool onEvent(const irr::SEvent &event, pluginsData &datas) override;


    /**
     * @brief Updates the plugin with the latest data.
     * @param dataManager Reference to the data manager.
     */
    void update(pluginsData dataManager) override;

    /**
     * @brief Gets the priority of the plugin.
     * @return The priority level of the plugin.
     */
    virtual int getPriority() const;
};
