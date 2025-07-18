#pragma once

#include <unordered_map>
#include <utility>

#include "modules/AIModule.hpp"
#include "Data/Data.hpp"

/**
 * @class AdvancedLeveler
 * @brief IA module which manage the leveling up process from level 3 to 8.
 *
 * This modules is low prio by default, it will awaken itself when the
 * conditions to start the process of the Elevation ritual are satisfied.
 *
 * Once awake, it asks all of the other AIs their inventory content, this will
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

    typedef struct ElevationRequirements_s {
        int playerCount;
        std::unordered_map<AI::Data::Material_t, int> materialsCount;
    } ElevationRequirements_t;

    typedef enum ModuleState_e {
        Idling,
        Answering,
        Listening,
        Calling,
        Moving,
        Spitting,
        Elevating
    } ModuleState_t;

 private:
    ModuleState_t _moduleState = Idling;
    std::unordered_map<int, AI::Data::Inventory_t> _othersInv;
    bool _isCaller;

    double computePriority(int level,
        const std::unordered_map<AI::Data::Material_t, int> &inventory);
};
