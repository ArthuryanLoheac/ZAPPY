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
        irr::core::rect<irr::s32> rect; /**< Rectangle defining the button's area. */
        bool hover = false;
        bool blocked = false;

        void draw(std::shared_ptr<irr::gui::IGUIFont> font,
          irr::video::IVideoDriver* driver, PlayerDataPlugin plug) {
            if (!font || !driver)
                return;
            plug.drawImage("assets/UI/AllRed.png",
              rect.UpperLeftCorner.X, rect.UpperLeftCorner.Y,
              rect.getWidth(), rect.getHeight(), driver, blocked ? 50 : (hover ? 120 : 255));
            font->draw(name.c_str(), rect, irr::video::SColor(blocked ? 100 : 255, 255, 255, 255), true, true);
        }

        bool isHover(irr::IrrlichtDevice *device) {
            hover = rect.isPointInside(device->getCursorControl()->getPosition());
            return hover;
        }

        void updatePos(const irr::core::position2d<irr::s32> &pos) {
            rect = irr::core::rect<irr::s32>(pos.X, pos.Y,
              pos.X + rect.getWidth(), pos.Y + rect.getHeight());
        }
    };

    Button LevelUpButton = Button("Level Up",
      irr::core::rect<irr::s32>(0, 0, 150, 30));

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
    bool onEvent(const irr::SEvent &event, pluginsData datas, std::string &outBuffer) override;

    std::string getName() const override {
        return "Player Data Plugin";
    }

    int getPriority() const override {
        return 1;
    }
};
