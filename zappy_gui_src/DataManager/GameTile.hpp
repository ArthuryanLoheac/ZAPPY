#pragma once
#include <string>
#include <vector>

namespace GUI {
class GameTile {
 public:
    class Egg {
     public:
        std::string team;
    };

    int x;
    int y;
    std::vector<Egg> eggs;
    GameTile(int xCoord, int yCoord) : x(xCoord), y(yCoord) {}
};
}  // namespace GUI
