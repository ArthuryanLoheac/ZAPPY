#pragma once
#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/pluginsInterface.hpp"

class TileDataPlugin : public pluginsInterface {
 private:
    pluginsData data; /**< Data manager for the plugin. */
    int xTile = -1;
    int yTile = -1;

    std::vector<UICol> lstColors = {
        UICol(255, 255, 255, 255), UICol(255, 200, 193, 198),
        UICol(255, 105, 105, 105), UICol(255, 121, 123, 166),
        UICol(255, 94, 84, 33), UICol(255, 144, 81, 82),
        UICol(255, 114, 85, 144)
    };

    std::vector<std::string> lstNames = {"Food", "Linemate", "Deraumere",
            "Sibur", "Mendiane", "Phiras", "Thystame"};

    irr::scene::ISceneManager* smgr;
    irr::IrrlichtDevice *device;
    irr::scene::ICameraSceneNode *cam;

    bool pressed = false;
    bool isPressedLastFrame = false;

   void drawOneBackground(const std::string &texture, int x, int y,
      int sizeX, int sizeY, irr::video::IVideoDriver* driver);

    pluginsData::Tile getTile(int x, int y);
    void detectCollisionTile();
    bool detectCollisionPlayer();

 public:
    bool init(irr::scene::ISceneManager* smgr,
      irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) override;
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;
    void onEvent(const irr::SEvent &event) override;
    void update(pluginsData &dataManager) override;
};