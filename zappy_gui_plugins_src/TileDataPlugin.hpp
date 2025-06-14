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
        UICol(255, 55, 55, 55), UICol(255, 71, 73, 116),
        UICol(255, 94, 84, 33), UICol(255, 94, 31, 32),
        UICol(255, 64, 35, 94)
    };

    std::vector<std::string> lstNames = {"Food", "Linemate", "Deraumere",
            "Sibur", "Mendiane", "Phiras", "Thystame"};

    irr::scene::ISceneManager* smgr;
    irr::IrrlichtDevice *device;
    irr::scene::ICameraSceneNode *cam;

    bool isPressedLastFrame;

   void drawOneBackground(const std::string &texture, int x, int y,
      int sizeX, int sizeY, irr::video::IVideoDriver* driver);

    pluginsData::Tile getTile(int x, int y);
    void detectCollisionTile();

 public:
    bool init(irr::scene::ISceneManager* smgr,
      irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) override;
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;
    void onEvent(const irr::SEvent &event) override;
    void update(pluginsData &dataManager) override;
};