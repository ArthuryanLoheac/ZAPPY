#pragma once
#include <string>

namespace GUI {
class ClientGui {
 public:
    explicit ClientGui(int _clientFd);
    int clientFd;
};
}  // namespace GUI
