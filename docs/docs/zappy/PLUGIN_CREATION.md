# Creating a Plugin

This guide explains how to create a shared object (`.so`) plugin for the project. These plugins are specifically designed for the GUI to display additional UI elements and interact with game data.

---

## 🛠️ 1. Requirements

Ensure you have the following installed:
- A C++ compiler (e.g., `g++` or `clang++`)
- The Irrlicht Engine development libraries
- The necessary headers and libraries for the project

---

## 📄 2. Plugin Structure

A plugin must implement the `pluginsInterface` class defined in the project. This interface ensures that the plugin can interact with the GUI and game data. The key methods are:

```cpp
class pluginsInterface {
 public:
    virtual ~pluginsInterface() = default;
    virtual bool init(irr::scene::ISceneManager* smgr,
        irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) = 0;
    virtual void update(pluginsData &dataManager) = 0;
    virtual void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver) = 0;
    virtual void onEvent(const irr::SEvent &event) = 0;
};
```

---

## 📝 3. Writing the Plugin

1. Create a new `.cpp` file for your plugin, e.g., `my_plugin.cpp`.
2. Implement the `pluginsInterface` methods:

```cpp
#include "PluginsManagement/pluginsInterface.hpp"

class MyPlugin : public pluginsInterface {
 public:
    bool init(irr::scene::ISceneManager* smgr,
        irr::IrrlichtDevice *device, irr::scene::ICameraSceneNode *cam) override {
        // Initialization code for setting up UI elements
        return true;
    }

    void update(pluginsData &dataManager) override {
        // Update logic using game data
    }

    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver) override {
        // Render additional UI elements
    }

    void onEvent(const irr::SEvent &event) override {
        // Handle user input or events
    }
};
```

---

## 🏗️ 4. Compiling the Plugin

### Manual Compilation

Use the following command to compile your plugin into a `.so` file:

```bash
g++ -shared -fPIC -o my_plugin.so my_plugin.cpp -I<path_to_headers> -lIrrlicht
```

- Replace `<path_to_headers>` with the path to the project's header files.
- Ensure the `-lIrrlicht` flag links the Irrlicht library.

### Using the Makefile

The project's Makefile includes a `plugins_all` target to automate plugin compilation. Place your `.cpp` file in the `zappy_gui_plugins_src` directory, and run:

```bash
make plugins_all
```

This will:
- Compile all `.cpp` files in `zappy_gui_plugins_src` into `.so` files.
- Place the resulting `.so` files in the `plugins/` directory.

---

## 🧪 5. Testing the Plugin

1. Place the `.so` file in the designated plugin directory.
2. Run the application and ensure it loads the plugin correctly.
3. Verify that the additional UI elements are displayed as expected.

---

## 📂 6. Example Directory Structure

```
project/
├── plugins/
│   ├── my_plugin.so
├── src/
├── include/
├── zappy_gui_plugins_src/
│   ├── my_plugin.cpp
└── ...
```

---

## 🔗 7. Additional Notes

- Use the `pluginsData` object to access game data such as players, tiles, and teams.
- Ensure your plugin adheres to the project's coding standards.
- Test your plugin thoroughly to ensure it does not interfere with existing UI components.
