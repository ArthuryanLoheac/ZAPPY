#pragma once

#include <string>
#include <vector>
#include <memory>

#include "dlLoader/dlLoader.hpp"
#include "pluginsInterface.hpp"

class pluginsManager {
 public:
    static pluginsManager &i() {
        static pluginsManager instance;
        return instance;
    }

    void loadPlugins(const std::string &path);
    void loadPlugin(const std::string &path);

 private:
    std::vector<std::unique_ptr<pluginsInterface>> _plugins;
};
