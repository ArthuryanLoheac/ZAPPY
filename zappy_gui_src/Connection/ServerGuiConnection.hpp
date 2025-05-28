#pragma once
#include <string>

namespace GUI
{
class ServerGuiConnection {
 public:
    static ServerGuiConnection &i() {
        static ServerGuiConnection i;
        return i;
    }
    void handleCommand(std::string &command);
};

} // namespace GUI