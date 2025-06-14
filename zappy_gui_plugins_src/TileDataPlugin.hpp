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

    irr::scene::ISceneManager* smgr;
    irr::IrrlichtDevice *device;
    irr::scene::ICameraSceneNode *cam;

   void drawOneBackground(const std::string &texture, int x, int y,
      int sizeX, int sizeY, irr::video::IVideoDriver* driver);

    pluginsData::Tile getTile(int x, int y);

 public:
    bool init(irr::scene::ISceneManager* smgr,
      irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) override;
    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
      irr::video::IVideoDriver* driver) override;
    void onEvent(const irr::SEvent &event) override;
    void update(pluginsData &dataManager) override;
};