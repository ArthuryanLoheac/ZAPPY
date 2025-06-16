#include <vector>

#include "SoundsManager.hpp"
#include "Exceptions/SoundsManagerExceptions.hpp"
#include "DataManager/GameDataManager.hpp"
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
        printf("------------ Playing sound: %s\n", path.c_str());
        for (size_t i = 0; i < sounds.size(); i++) {
            if (sounds[i].getStatus() == sf::Sound::Stopped) {
                sounds.erase(sounds.begin() + i);
                soundBuffers.erase(soundBuffers.begin() + i);
                i--;
            }
        }
        soundBuffers.push_back(sf::SoundBuffer());
        if (!soundBuffers.back().loadFromFile(path))
            throw SoundLoadingException(path);
        sounds.push_back(sf::Sound(soundBuffers.back()));
        sounds.back().setVolume(100);
        sounds.back().play();
        printf("------------ Sound played successfully\n");
        return true;
    } catch (const std::exception &e) {
        LOG_ERROR("Unexpected error: %s", e.what());
        return false;
    }
}
void SoundsManager::Update() {
    try {
        for (size_t i = 0; i < sounds.size(); i++) {
            if (sounds[i].getStatus() == sf::Sound::Stopped) {
                sounds.erase(sounds.begin() + i);
                soundBuffers.erase(soundBuffers.begin() + i);
                i--;
            }
        }
    } catch (const std::exception &e) {
        LOG_ERROR("Error updating sounds: %s", e.what());
    }
    if (GameDataManager::i().isPlayerAdded()) {
        GameDataManager::i().setPlayerAdded(false);
        playSound("assets/Musics/Spawn.wav");
    }
}
}  // namespace GUI
