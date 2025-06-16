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
        music.setVolume(volumeMusic);
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
                soundBuffers.erase(soundBuffers.begin() + i);
                i--;
            }
        }
        soundBuffers.push_back(sf::SoundBuffer());
        if (!soundBuffers.back().loadFromFile(path))
            throw SoundLoadingException(path);
        sounds.push_back(sf::Sound(soundBuffers.back()));
        sounds.back().setVolume(volumeSound);
        sounds.back().play();
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
    if (GameDataManager::i().isElevation()) {
        GameDataManager::i().setElevationSound(false);
        playSound("assets/Musics/Elevation.wav");
    }
    if (GameDataManager::i().isPlayerDead()) {
        GameDataManager::i().setPlayerDead(false);
        playSound("assets/Musics/Death.mp3");
    }
    if (GameDataManager::i().isCollecting()) {
        GameDataManager::i().setCollecting(false);
        playSound("assets/Musics/Take.wav");
    }
    if (GameDataManager::i().isDropping()) {
        GameDataManager::i().setDropping(false);
        playSound("assets/Musics/Drop.wav");
    }
    if (GameDataManager::i().isEggDead()) {
        GameDataManager::i().setEggDead(false);
        playSound("assets/Musics/DeathEgg.wav");
    }
    if (GameDataManager::i().isEggAdded()) {
        GameDataManager::i().setEggAdded(false);
        playSound("assets/Musics/SpawnEgg.wav");
    }
    if (GameDataManager::i().isPushed()) {
        GameDataManager::i().setPushed(false);
        playSound("assets/Musics/Push.wav");
    }
}
}  // namespace GUI
