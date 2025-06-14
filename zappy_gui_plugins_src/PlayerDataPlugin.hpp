#pragma once
#include <string>
#include <memory>
#include <vector>

#include "zappy_gui_src/PluginsManagement/pluginsInterface.hpp"

class PlayerDataPlugin : public pluginsInterface {
 private:
    pluginsData data; /**< Data manager for the plugin. */
    int idPlayer = -1;

    irr::scene::ISceneManager* smgr;
    irr::IrrlichtDevice *device;
    irr::scene::ICameraSceneNode *cam;
    irr::video::IVideoDriver* driver;

    bool pressed = false;
    bool isPressedLastFrame = false;

    std::vector<std::string> lstNames = {"Food", "Linemate", "Deraumere",
        "Sibur", "Mendiane", "Phiras", "Thystame"};

    std::vector<UICol> lstColors = {
        UICol(255, 255, 255, 255), UICol(255, 200, 193, 198),
        UICol(255, 105, 105, 105), UICol(255, 121, 123, 166),
        UICol(255, 94, 84, 33), UICol(255, 144, 81, 82),
        UICol(255, 114, 85, 144)};

   void drawOneBackground(const std::string &texture, int x, int y,
      int sizeX, int sizeY, irr::video::IVideoDriver* driver);

    pluginsData::Tile getTile(int x, int y);
    void detectCollisionPlayer();
    void drawPlayerInfo(int id, std::shared_ptr<irr::gui::IGUIFont> font);

 public:
    bool init(irr::scene::ISceneManager* smgr,
      irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) override;
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;
    void onEvent(const irr::SEvent &event) override;
    void update(pluginsData &dataManager) override;
};