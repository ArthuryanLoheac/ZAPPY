#pragma once

#include <unordered_map>
#include <string>

#include "modules/AIModule.hpp"

/**
 * @class AdvancedLeveler
 * @brief IA module which manage the leveling up process from level 3 to 8.
 *
 * This modules is low prio by default, it will awaken itself when the
 * conditions to start the process of the Elevation ritual are satisfied.
 *
 * Once awaken, it ask all of the other AIs their inventory content, this will
 * allow the levelers to know if the total of required ressources is satisfied.
 * If this total amount of resources is satifying, the leveler with the lowest
 * level will start spamming broadcasts to allow the other AIs to reunite
 * in his tile.
 * Once reunited, they will all drop their ressources on the ground and all
 * start the Elevation ritual.
 *
 * Once all of these steps are completed, the module will go back to low prio.
 *
 * The module is designed to be used as part of a larger AI system, inheriting
 * from AIModule, and interacts with the AI interface and data layers.
 */
class AdvancedLeveler : public AIModule {
 public:

    AdvancedLeveler() = default;
    ~AdvancedLeveler() override = default;

    /**
     * @brief Execute the module's main action.
     *
     * Sends a broadcast message as Frank Leboeuf.
     */
    void execute() override;

    /**
     * @brief Get the priority of this module.
     *
     * @return The priority value as a float.
     */
    float getPriority() override;

    typedef enum Material_e {
        Linemate,
        Deraumere,
        Sibur,
        Mendiane,
        Phiras,
        Thystame
    } Material_t;

    typedef struct ElevationRequirements_s {
        int playerCount;
        std::unordered_map<Material_t, int> materialsCount;
    } ElevationRequirements_t;
 private:
    double computePriority(int level, const std::unordered_map
        <Material_t, int> &inventory);
};
