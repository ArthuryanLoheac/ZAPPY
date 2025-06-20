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
    virtual void update(pluginsData dataManager) = 0;
    virtual void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver) = 0;
    virtual bool onEvent(const irr::SEvent &event, pluginsData &datas) = 0;
    virtual int priority() const = 0; // New method to define plugin priority
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

    void update(pluginsData dataManager) override {
        // Update logic using game data
    }

    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font,
        irr::video::IVideoDriver* driver) override {
        // Render additional UI elements
    }

    bool onEvent(const irr::SEvent &event, pluginsData &datas) override {
        // Handle user input or events
        // return true if the event is consumed
        // datas is a ref to the structure datas (used for send datas to serv)
        // Updated logic for handling events
    }

    int priority() const override {
        // Return the priority of the plugin
        // Greater the value faster it will be taken
        return 10; // Example priority value
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
- The `priority` method allows the GUI to determine the order in which plugins are processed. Higher priority values are processed first.

---

## 🧩 8. Example Plugin Creation

Below is an example of how to create a plugin and expose it for the application:

```cpp
#include "zappy_gui_src/PluginsManagement/pluginsInterface.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class ExamplePlugin : public pluginsInterface {
 private:
    pluginsData data; /**< Data manager for the plugin. */
    irr::scene::ISceneManager* smgr; /**< Pointer to the Irrlicht scene manager. */
    irr::IrrlichtDevice* device; /**< Pointer to the Irrlicht device. */
    irr::scene::ICameraSceneNode* cam; /**< Pointer to the Irrlicht camera. */
    irr::video::IVideoDriver* driver; /**< Pointer to the video driver. */
    bool isActive = true; /**< Indicates if the plugin is active. */

    void drawImage(const std::string& texture, int x, int y, int sizeX, int sizeY, irr::video::IVideoDriver* driver) {
        irr::video::ITexture* bg = driver->getTexture(texture.c_str());
        irr::core::rect<irr::s32> sourceRect(0, 0, 1000, 1000);
        irr::core::rect<irr::s32> destRect(x, y, x + sizeX, y + sizeY);
        if (!bg) {
            std::cerr << "Error: Texture not found: " << texture << std::endl;
            return;
        }
        driver->draw2DImage(bg, destRect, sourceRect, 0, nullptr, true);
    }

 public:
    bool init(irr::scene::ISceneManager* _smgr, irr::IrrlichtDevice* _device, irr::scene::ICameraSceneNode* _cam) override {
        smgr = _smgr;
        device = _device;
        cam = _cam;
        std::cout << "ExamplePlugin initialized successfully!" << std::endl;
        return true;
    }

    void update(pluginsData& _data) override {
        data = _data;
        std::cout << "ExamplePlugin updated with new data!" << std::endl;
    }

    void drawUI(std::shared_ptr<irr::gui::IGUIFont> font, irr::video::IVideoDriver* _driver) override {
        driver = _driver;
        if (!font || !driver || !isActive) return;

        UICol white(255, 255, 255, 255);

        drawImage("assets/UI/ExampleBackground.png", 0, 0, 150, 400, driver, 125);

        // Example data display
        font->draw(("FPS : " + std::to_string(driver->getFPS())).c_str(), UIRect(30, 30, 300, 50), white);
        font->draw(("Freq : " + std::to_string(data.freq)).c_str(), UIRect(130, 30, 300, 50), white);
    }

    void onEvent(const irr::SEvent& event) override {
        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
            if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
                std::cout << "ExamplePlugin: Left mouse button pressed!" << std::endl;
                isActive = !isActive;
            }
        }
    }
    
    int getPriority() {
      return 10;
    }
};

// Exposing the plugin
extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<ExamplePlugin>();
    }
}

```

This example demonstrates:
- Implementing the `pluginsInterface` methods.
- Using `extern "C"` to expose the plugin creation function.

Place the `.cpp` file in the `zappy_gui_plugins_src` directory and compile it using the project's Makefile or manually as described earlier.
