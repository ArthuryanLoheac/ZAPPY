#pragma once

namespace AI {

class Data {
 public:
    Data();
    ~Data();

    static Data &i() {
        static Data instance;
        return instance;
    }

    bool isDead;
    int mapX;
    int mapY;

 private:
};

}  // namespace AI
