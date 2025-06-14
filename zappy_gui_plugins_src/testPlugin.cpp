#include "testPlugin.hpp"

#include <memory>
#include <string>
#include <vector>

extern "C" {
    std::unique_ptr<pluginsInterface> createPlugin() {
        return std::make_unique<testPlugin>();
    }
}

bool testPlugin::init() {
    printf("============= Initializing Test Plugin =============\n");
    return true;
}

const char* testPlugin::getName() const {
    return "Test Plugin";
}

const char* testPlugin::getVersion() const {
    return "1.0.0";
}

void testPlugin::drawUI(std::shared_ptr<irr::gui::IGUIFont> font) {
    if (!font)
        return;
    font->draw(L"Test Plugin UI", irr::core::rect<irr::s32>(200, 10, 200, 50),
               irr::video::SColor(255, 255, 255, 255));
}
