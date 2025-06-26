#pragma once
#include <irrlicht/irrlicht.h>

#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/include/Aplugin.hpp"

/**
 * @class TileDataPluginDebug
 * @brief Plugin class for handling tile data visualization and interaction.
 * @implements Aplugin
 */
class TileDataPluginDebug : public Aplugin {
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
          irr::video::IVideoDriver* driver);

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

    int xTile = -1; /**< X position of the selected tile. */
    int yTile = -1; /**< Y position of the selected tile. */
    std::shared_ptr<irr::gui::IGUIFont> font;
    irr::video::IVideoDriver* driver;

    bool pressed = false; /**< Indicates if the mouse is currently pressed. */
    bool isPressedLastFrame = false;
      /**< Indicates if the mouse was pressed in the last frame. */

    Button FoodPlusButton = Button("Food +",
      irr::core::rect<irr::s32>(0, 0, 75, 30));
    Button FoodMinusButton = Button("Food -",
      irr::core::rect<irr::s32>(0, 0, 75, 30));
    Button LinematePlusButton = Button("Line +",
      irr::core::rect<irr::s32>(0, 0, 75, 30));
    Button LinemateMinusButton = Button("Line -",
      irr::core::rect<irr::s32>(0, 0, 75, 30));
    Button DeraumerePlusButton = Button("Dera +",
      irr::core::rect<irr::s32>(0, 0, 75, 30));
    Button DeraumereMinusButton = Button("Dera -",
      irr::core::rect<irr::s32>(0, 0, 75, 30));
    Button SiburPlusButton = Button("Sibu +",
      irr::core::rect<irr::s32>(0, 0, 75, 30));
    Button SiburMinusButton = Button("Sibu -",
      irr::core::rect<irr::s32>(0, 0, 75, 30));
    Button MendianePlusButton = Button("Mend +",
      irr::core::rect<irr::s32>(0, 0, 75, 30));
    Button MendianeMinusButton = Button("Mend -",
      irr::core::rect<irr::s32>(0, 0, 75, 30));
    Button PhirasPlusButton = Button("Phir +",
      irr::core::rect<irr::s32>(0, 0, 75, 30));
    Button PhirasMinusButton = Button("Phir -",
      irr::core::rect<irr::s32>(0, 0, 75, 30));
    Button ThystamePlusButton = Button("Thys +",
      irr::core::rect<irr::s32>(0, 0, 75, 30));
    Button ThystameMinusButton = Button("Thys -",
      irr::core::rect<irr::s32>(0, 0, 75, 30));

    /**
     * @brief Retrieves the tile data at the specified position.
     * @param x The X position of the tile.
     * @param y The Y position of the tile.
     * @return The tile data.
     */
    pluginsData::Tile getTile(int x, int y);

    /**
     * @brief Detects collision with a tile.
     */
    bool detectCollisionTile();
    bool handleClick(std::string &outBuffer, irr::core::vector2d<irr::s32> pos);
    bool updateDoubleButton(std::string ressourceName,
        TileDataPluginDebug::Button &bMinus, TileDataPluginDebug::Button &bPlus,
        std::string &outBuffer, irr::core::vector2d<irr::s32> pos);

    void drawDoubleButtons(TileDataPluginDebug::Button &bMinus,
        TileDataPluginDebug::Button &bPlus,
        std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver *driver, int ressource,
        pluginsData::Tile tile, int &y);

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
        return "Tile Debug Data Plugin";
    }

    int getPriority() const override {
        return 0;
    }
};
