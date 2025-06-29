#include "modules/HarvesterSpawner.hpp"

#include <unistd.h>

#include <string>

#include "Interface/Interface.hpp"
#include "Data/Data.hpp"
#include "../../libc/include/logs.h"

/**
 * @brief Constructs a new HarvesterSpawner object
 *
 * Initializes the module with default values for spawn counts and cooldown.
 */
HarvesterSpawner::HarvesterSpawner()
    : feederCount(0), harvesterCount(0),
      ticksUntilNextSpawn(100), isSpawnCooldown(false) {
    LOG_INFO("HarvesterSpawner module initialized");
}

/**
 * @brief Destroys the HarvesterSpawner object
 */
HarvesterSpawner::~HarvesterSpawner() = default;

/**
 * @brief Executes the main logic of the HarvesterSpawner module
 *
 * Attempts to spawn feeders and harvesters based on team needs.
 */
void HarvesterSpawner::execute() {
    if (isSpawnCooldown) {
        ticksUntilNextSpawn--;
        if (ticksUntilNextSpawn <= 0) {
            isSpawnCooldown = false;
            ticksUntilNextSpawn = 100;
        }
        return;
    }
    analyzeTeamNeeds();
    AI::Interface::i().sendCommand(LOOK);
}

/**
 * @brief Returns the priority of this module
 * @return The priority value
 */
float HarvesterSpawner::getPriority() {
    return 0.2f;
}

/**
 * @brief Analyzes team needs and determines what agents to spawn
 *
 * Checks food levels and team composition to decide whether to spawn
 * feeders or harvesters.
 */
void HarvesterSpawner::analyzeTeamNeeds() {
    int foodCount = AI::Data::i().inventory[AI::Data::Material_t::Food];

    // Need more food collectors if food is low
    if (foodCount < 4 && feederCount < 3) {
        if (spawnAgent("FEEDER")) {
            feederCount++;
            LOG_INFO("Spawned a feeder agent (total: %d)", feederCount);
            isSpawnCooldown = true;
            return;
        }
    }

    // Otherwise spawn harvesters
    if (foodCount >= 4 && harvesterCount < 4) {
        if (spawnAgent("HARVESTER")) {
            harvesterCount++;
            LOG_INFO("Spawned a harvester agent (total: %d)", harvesterCount);
            isSpawnCooldown = true;
            return;
        }
    }

    if (feederCount <= harvesterCount) {
        if (spawnAgent("FEEDER")) {
            feederCount++;
            LOG_INFO("Spawned additional feeder (total: %d)", feederCount);
            isSpawnCooldown = true;
        }
    } else {
        if (spawnAgent("HARVESTER")) {
            harvesterCount++;
            LOG_INFO("Spawned additional harvester (total: %d)",
                harvesterCount);
            isSpawnCooldown = true;
        }
    }
}

/**
 * @brief Attempts to spawn a new agent with the specified role
 *
 * @param role The role to assign to the new agent ("FEEDER" or "HARVESTER")
 * @return true if spawning was successful, false otherwise
 */
bool HarvesterSpawner::spawnAgent(const std::string& role) {
    int foodCount = AI::Data::i().inventory[AI::Data::Material_t::Food];
    if (foodCount <= 3) {
        LOG_INFO("Not enough food to spawn a %s agent", role.c_str());
        return false;
    }

    AI::Interface::i().sendCommand("Fork");

    if (role == "FEEDER") {
        AI::Interface::i().sendMessage("NEED_FEEDER");
        AI::Interface::i().sendMessage("NEED_FEEDER");
        AI::Interface::i().sendMessage("NEED_FEEDER");
        LOG_INFO("Broadcasting NEED_FEEDER for newly forked process");
    } else if (role == "HARVESTER") {
        AI::Interface::i().sendMessage("NEED_HARVESTER");
        AI::Interface::i().sendMessage("NEED_HARVESTER");
        AI::Interface::i().sendMessage("NEED_HARVESTER");
        LOG_INFO("Broadcasting NEED_HARVESTER for newly forked process");
    }

    return true;
}
