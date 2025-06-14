#include "testPlugin.hpp"

#include <memory>

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
