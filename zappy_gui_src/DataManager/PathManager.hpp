#pragma once
#include <map>
#include <string>

namespace GUI {
class PathManager {
 private:
    std::map<std::string, std::string> _paths;
        // Map to store paths with their associated keys
    std::map<std::string, float> _scales;
        // Map to store paths with their associated keys

    PathManager() {
        // Initialize paths with default values
        _paths["Food"] = "Battery";
        _paths["Ring"] = "Cylinder";
        _paths["Player"] = "Drone";
        _paths["Egg"] = "DroneEgg";
        _paths["Tile"] = "Plane";
        _paths["Mat1"] = "Mat1";
        _paths["Mat2"] = "Mat2";
        _paths["Mat3"] = "Mat3";
        _paths["Mat4"] = "Mat4";
        _paths["Mat5"] = "Mat5";
        _paths["Mat6"] = "Mat6";
        // Skybox
        _paths["skyboxTop"] = "skybox/top.png";
        _paths["skyboxBottom"] = "skybox/bottom.png";
        _paths["skyboxLeft"] = "skybox/left.png";
        _paths["skyboxRight"] = "skybox/right.png";
        _paths["skyboxFront"] = "skybox/front.png";
        _paths["skyboxBack"] = "skybox/back.png";
    }

    // Delete copy constructor and assignment operator
    PathManager(const PathManager &) = delete;
    PathManager &operator=(const PathManager &) = delete;

 public:
    static PathManager &i() {
        static PathManager instance;
        return instance;
    }

    /**
     * @brief Retrieves the path associated with a given key.
     * @param key The key for which to retrieve the path.
     * @return The path associated with the key, or an empty string if the key is not found.
     */
    std::string getPath(const std::string &key) const {
        auto it = _paths.find(key);
        if (it != _paths.end()) {
            return it->second;
        }
        return "";
    }

    /**
     * @brief Sets the path for a given key.
     * @param key The key for which to set the path.
     * @param value The path value to set.
     */
    void setPath(const std::string &key, const std::string &value) {
        _paths[key] = value;
    }

    /**
     * @brief Retrieves the scale associated with a given key.
     * @param key The key for which to retrieve the scale.
     * @return The scale associated with the key, or 1 if the key is not found.
     */
    float getScale(const std::string &key) const {
        auto it = _scales.find(key);
        if (it != _scales.end())
            return it->second;
        return 1;
    }

    /**
     * @brief Sets the scale for a given key.
     * @param key The key for which to set the scale.
     * @param value The scale value to set.
     */
    void setScale(const std::string &key, const float &value) {
        _scales[key] = value;
    }
};

}  // namespace GUI
