#pragma once
#include <vector>
#include <string>

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

    void Update();

    int volumeMusic = 50;
    int volumeSound = 50;
    void AddVolumeMusic(int volume);
    void AddVolumeSound(int volume);
 private:
    std::vector<sf::Sound> sounds;
    std::vector<sf::SoundBuffer> soundBuffers;
    sf::Sound musicActual;
    sf::Music music;
};
}  // namespace GUI
