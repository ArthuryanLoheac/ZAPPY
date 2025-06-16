#include <vector>

#include "SoundsManager.hpp"
#include "Exceptions/SoundsManagerExceptions.hpp"
#include "include/logs.h"
#include <iostream>

namespace GUI {
bool SoundsManager::playMusic(std::string path) {
    try {
        if (music.getStatus() == sf::SoundSource::Playing)
            music.stop();
        if (!music.openFromFile(path))
            throw MusicLoadingException(path);
        music.setLoop(true);
        music.play();
        return true;
    } catch (const std::exception &e) {
        LOG_ERROR("Error playing music: %s", e.what());
        return false;
    }
}

bool SoundsManager::playSound(std::string path) {
    try {
        for (size_t i = 0; i < sounds.size(); i++) {
            if (sounds[i].getStatus() == sf::Sound::Stopped) {
                sounds.erase(sounds.begin() + i);
                i--;
            }
        }
        sf::SoundBuffer s;
        if (!s.loadFromFile(path))
            throw SoundLoadingException(path);
        sf::Sound sound(s);
        sound.play();
        sounds.push_back(sound);
        return true;
    } catch (const SoundsManagerException &e) {
        LOG_ERROR("Error playing sound: %s", e.what());
        return false;
    } catch (const std::exception &e) {
        LOG_ERROR("Unexpected error: %s", e.what());
        return false;
    }
}
}  // namespace GUI
