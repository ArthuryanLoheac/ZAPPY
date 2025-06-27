#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/include/Aplugin.hpp"

/**
 * @class MessagesPlugin
 * @brief Plugin class for managing global data visualization and interaction.
 * @implements Aplugin
 */
class MessagesPlugin : public Aplugin {
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

    int speedParticle = 1;
    std::vector<Particle> particles;

    /**
     * @brief Draws a message history on the screen.
     * @param font Shared pointer to the GUI font.
     * @param driver Pointer to the video driver.
     */
    void drawMessageHistory(std::shared_ptr<irr::gui::IGUIFont> font,
         irr::video::IVideoDriver* driver);

    /**
     * @brief Initializes a particle with a position, direction, and age.
     * @param driver Pointer to the video driver.
     * @param position The initial position of the particle.
     * @param direction The initial direction of the particle.
     * @param age The age of the particle, which determines its lifetime.
     */
    void initParticle(irr::video::IVideoDriver* driver,
        irr::core::vector3df position, irr::core::vector3df direction, int age);
    /**
     * @brief Checks and deletes particles that have reached their end time.
     * This function iterates through the particles and removes those that have
     * expired or stopped emitting.
     */
    void checkDeleteParticles();

    /**
     * @brief Sends particles to all players based on the message data.
     * @param driver Pointer to the video driver.
     * @param p The player data for which particles are sent.
     */
    void SendParticlesToAll(irr::video::IVideoDriver* driver,
        pluginsData::Player &p);

 public:
    /**
     * @brief Draws the UI elements for the plugin.
     * @param font Shared pointer to the GUI font.
     * @param driver Pointer to the video driver.
     */
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;

    std::string getName() const override {
        return "Messages Plugin";
    }
};
