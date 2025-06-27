#pragma once

#include "modules/AIModule.hpp"

#define FOOD_TICK 126

/**
 * @class KirbyModule
 * @brief AI module that simulates a "Kirby"-like behavior for the Zappy AI.
 *
 * The KirbyModule is responsible for a specific AI behavior where the agent
 * moves forward collecting items ("suck" mode), then returns to the starting
 * position to drop the collected items ("spit" mode). The module intelligently
 * chooses between turning around or looping around the map based on efficiency.
 * It manages its own state, including steps taken, objects collected, time
 * tracking, and mode switching based on remaining time and safety margins.
 *
 * The module operates in two phases:
 * - Suck mode: Move forward, look around, and collect non-food items
 * - Spit mode: Return to start (either by turning back or looping) and drop items
 *
 * The module is designed to be used as part of a larger AI system, inheriting
 * from AIModule, and interacts with the AI interface and data layers.
 */
class KirbyModule : public AIModule {
 public:
    /**
     * @brief Constructs a new KirbyModule object.
     */
    KirbyModule();

    /**
     * @brief Destroys the KirbyModule object.
     */
    ~KirbyModule();

    /**
     * @brief Executes the main logic of the KirbyModule.
     *
     * Decides whether to perform the suck or spit action based on the current mode.
     */
    void execute() override;

    /**
     * @brief Returns the priority of this module.
     * @return The priority as a float value.
     */
    float getPriority() override;

    /**
     * @brief Computes whether the KirbyModule should remain in suck mode.
     *
     * Updates the suckMode flag based on time and actions left.
     */
    void computeSuckMode();

    /**
     * @brief Performs the "suck" action: looks, takes objects, and moves forward.
     */
    void suck();

    /**
     * @brief Performs the "spit" action: returns to starting position and drops collected objects.
     * 
     * Chooses the most efficient return path (turning back vs looping around) and
     * drops all non-food items from inventory.
     */
    void spit();

    /**
     * @brief Takes all objects from the current cell (after looking).
     * 
     * Prioritizes non-food items first, then collects food items.
     */
    void takeObjects();

    /**
     * @brief Gets the total number of non-food objects in the inventory.
     * @return The number of objects as an integer (excluding food).
     */
    int getNbObjects();

 private:
    int tickUsed;              ///< Number of ticks consumed so far
    int timeRemaining;         ///< Total time budget for the module
    int forwardCount;          ///< Number of forward steps taken
    bool suckMode;             ///< True if in collection mode, false if returning
    bool hasMadeHisWill;       ///< True if return journey has been initiated
    bool shouldLoopAround;     ///< True if should loop around map instead of turning back
};
