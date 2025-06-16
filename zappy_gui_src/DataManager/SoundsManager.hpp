#pragma once
#include <vector>

#include <SFML/Audio.hpp>

namespace GUI {
class SoundsManager {
 public:
    static SoundsManager &i() {
        static SoundsManager instance;
        return instance;
    }

    bool playMusic(std::string path);
    bool playSound(std::string path);

 private:
    std::vector<sf::Sound> sounds;
    sf::Sound musicActual;
    sf::Music music;
};
}  // namespace GUI
