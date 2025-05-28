#pragma once
#include <string>

namespace GUI {
class ClientGui {
 public:
    ClientGui(int _clientFd);
    int clientFd;
};
}  // namespace GUI