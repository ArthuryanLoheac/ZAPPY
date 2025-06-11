#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace AI {

class Data {
 public:
    Data();
    ~Data();

    static Data &i() {
        static Data instance;
        return instance;
    }

    // basics
    bool isRunning;
    bool isDead;
    int mapX;
    int mapY;

    // representation of the vision given by the LOOK command, first vector for
    // depth, second for the direction and third is the objects and the number
    // of this object
    std::vector<std::vector<std::unordered_map<std::string, int>>> vision;
    // This method return the objects at the given coordinates in the vision,
    // where x is the depth and y is the direction (for the direction, it is
    // a negative number for the left, 0 for the middle and a positive for the
    // right).
    std::unordered_map<std::string, int> &mapAt(int x, int y);

 private:
};

}  // namespace AI
