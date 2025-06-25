#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/include/Aplugin.hpp"

/**
 * @class PlayerDataPlugin
 * @brief Plugin class for managing player data visualization and interaction
 * when clicking on one
 * @implements Aplugin
 */
class PlayerDataPlugin : public Aplugin {
 private:
    class Button {
     public:
        Button(const std::string &name, const irr::core::rect<irr::s32> &rect)
        : name(name), rect(rect) {}

        std::string name; /**< Name of the button. */
        irr::core::rect<irr::s32> rect; /**< Rectangle defining the button's. */
        bool hover = false; /**< Indicates if the button is hovered. */
        bool blocked = false; /** Indicates if the button is blocked. */

        /** @brief Draws the button on the screen.
         * @param font Shared pointer to the GUI font.
         * @param driver Pointer to the video driver.
         * @param plug Reference to the PlayerDataPlugin instance.
         */
        void draw(std::shared_ptr<irr::gui::IGUIFont> font,
          irr::video::IVideoDriver* driver, PlayerDataPlugin plug);

        /**
         * @brief Checks if the button is hovered by the mouse.
         * @param device Pointer to the Irrlicht device.
         * @return True if the button is hovered, false otherwise.
         */
        bool isHover(irr::core::vector2di pos);

        /** @brief Updates the button's position.
         * @param pos The new position of the button.
         */
        void updatePos(const irr::core::position2d<irr::s32> &pos);
    };

    Button LevelUpButton = Button("Level Up",
      irr::core::rect<irr::s32>(0, 0, 150, 30));
    Button ForwardButton = Button("F",
      irr::core::rect<irr::s32>(0, 0, 30, 30));
    Button BackwardButton = Button("B",
      irr::core::rect<irr::s32>(0, 0, 30, 30));
    Button LeftButton = Button("L",
      irr::core::rect<irr::s32>(0, 0, 30, 30));
    Button RightButton = Button("R",
      irr::core::rect<irr::s32>(0, 0, 30, 30));
    Button ForkButton = Button("Fork",
      irr::core::rect<irr::s32>(0, 0, 60, 30));
    Button EjectButton = Button("Eject",
      irr::core::rect<irr::s32>(0, 0, 60, 30));

    int idPlayer = -1; /**< ID of the selected player. */

    bool pressed = false; /**< Indicates if the mouse is currently pressed. */
    bool isPressedLastFrame = false;
      /**< Indicates if the mouse was pressed in the last frame. */

    /**
     * @brief Retrieves the tile data at the specified position.
     * @param x The X position of the tile.
     * @param y The Y position of the tile.
     * @return The tile data.
     */
    pluginsData::Tile getTile(int x, int y);

    /**
     * @brief Detects collision with a player.
     */
    bool detectCollisionPlayer();

    /**
     * @brief Draws information about a specific player.
     * @param id The ID of the player.
     * @param font Shared pointer to the GUI font.
     */
    void drawPlayerInfo(int id, std::shared_ptr<irr::gui::IGUIFont> font);

    bool handleClick(std::string &outBuffer, irr::core::vector2d<irr::s32> pos);

 public:
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
    bool onEvent(const irr::SEvent &event, pluginsData datas,
        std::string &outBuffer) override;

    std::string getName() const override {
        return "Player Data Plugin";
    }

    int getPriority() const override {
        return 1;
    }
};
