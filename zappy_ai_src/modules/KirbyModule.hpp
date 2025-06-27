#pragma once

#include "modules/AIModule.hpp"

#define FOOD_TICK 126

/**
 * @class KirbyModule
 * @brief AI module that simulates a "Kirby"-like behavior for the Zappy AI.
 *
 * The KirbyModule is responsible for a specific AI behavior where the agent
 * "sucks" up objects by moving forward and collecting items, then "spits"
 * them out by retracing its steps and dropping the collected items. The module
 * manages its own state, including the number of steps taken, objects collected,
 * and whether it is in suck or spit mode. It also tracks time and ticks used
 * for its actions.
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
     * @brief Performs the "spit" action: retraces steps and drops collected objects.
     */
    void spit();

    /**
     * @brief Takes all objects from the cell in front of the agent.
     */
    void takeObjects();

    /**
     * @brief Gets the total number of objects in the inventory.
     * @return The number of objects as an integer.
     */
    int getNbObjects();

 private:
    int tickUsed;
    int timeRemaining;
    int forwardCount;
    bool suckMode;
    bool hasMadeHisWill;
    bool shouldLoopAround;
};
