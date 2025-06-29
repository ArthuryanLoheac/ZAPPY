#pragma once

#include <vector>
#include <string>

#include "modules/AIModule.hpp"

/**
 * @class HarvesterSpawner
 * @brief Module that spawns feeder and harvester AI agents
 *
 * This module is responsible for creating feeder and harvester units
 * but does not pick up items. It's used after the KirbyModule completes
 * its resource collection phase.
 */
class HarvesterSpawner : public AIModule {
 public:
    /**
     * @brief Constructor for HarvesterSpawner
     */
    HarvesterSpawner();

    /**
     * @brief Destructor for HarvesterSpawner
     */
    ~HarvesterSpawner();

    /**
     * @brief Execute the harvester spawning behavior
     */
    void execute() override;

    /**
     * @brief Calculate the priority of this module
     * @return float Priority value
     */
    float getPriority() override;

 private:
    /**
     * @brief Spawn a new AI agent with the specified role
     * @param role The role to assign to the new agent
     * @return bool Whether spawning was successful
     */
    bool spawnAgent(const std::string& role);

    /**
     * @brief Analyze team needs and determine what to spawn next
     */
    void analyzeTeamNeeds();

    int feederCount;     ///< Number of feeders spawned
    int harvesterCount;  ///< Number of harvesters spawned
    int ticksUntilNextSpawn;  ///< Ticks until next spawn attempt
    bool isSpawnCooldown;     ///< Whether we're in spawn cooldown
};
