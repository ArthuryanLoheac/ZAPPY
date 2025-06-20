
#include <vector>
#include <iostream>
#include <string>

#include "include/logs.h"
#include "DataManager/SoundsManager.hpp"
#include "Exceptions/SoundsManagerExceptions.hpp"
#include "DataManager/GameDataManager.hpp"
#include "DataManager/PathManager.hpp"

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
        playSound("assets/" + GUI::PathManager::i().getPath("Spawn"));
    }
    if (GameDataManager::i().isElevation()) {
        GameDataManager::i().setElevationSound(false);
        playSound("assets/" + GUI::PathManager::i().getPath("Elevation"));
    }
    if (GameDataManager::i().isPlayerDead()) {
        GameDataManager::i().setPlayerDead(false);
        playSound("assets/" + GUI::PathManager::i().getPath("Death"));
    }
    if (GameDataManager::i().isCollecting()) {
        GameDataManager::i().setCollecting(false);
        playSound("assets/" + GUI::PathManager::i().getPath("Take"));
    }
    if (GameDataManager::i().isDropping()) {
        GameDataManager::i().setDropping(false);
        playSound("assets/" + GUI::PathManager::i().getPath("Drop"));
    }
    if (GameDataManager::i().isEggDead()) {
        GameDataManager::i().setEggDead(false);
        playSound("assets/" + GUI::PathManager::i().getPath("DeathEgg"));
    }
    if (GameDataManager::i().isEggAdded()) {
        GameDataManager::i().setEggAdded(false);
        playSound("assets/" + GUI::PathManager::i().getPath("SpawnEgg"));
    }
    if (GameDataManager::i().isPushed()) {
        GameDataManager::i().setPushed(false);
        playSound("assets/" + GUI::PathManager::i().getPath("Push"));
    }
}
}  // namespace GUI
