/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Resource Gathering and Spawning Module
*/

#ifndef ZAPPY_AI_SRC_MODULES_RESSOURCEGATHERINGSPAWNING_HPP_
#define ZAPPY_AI_SRC_MODULES_RESSOURCEGATHERINGSPAWNING_HPP_

#include "modules/AIModule.hpp"

/**
 * @brief Module responsible for creating new players and assigning roles
 *
 * This module forks new players when resources are sufficient and
 * broadcasts role assignments to the newly spawned players.
 */
class RessourceGatheringSpawning : public AIModule {
 public:
    /**
     * @brief Constructor for the RessourceGatheringSpawning module
     */
    RessourceGatheringSpawning();

    /**
     * @brief Execute the spawning behavior
     */
    void execute() override;

    /**
     * @brief Calculate the priority of this module
     * @return float Priority value between 0.0 and 1.0
     */
    float getPriority() override;

 private:
    /**
     * @brief Check if we have enough resources to spawn
     * @return true if we have sufficient resources
     */
    bool hasSufficientResources() const;

    /**
     * @brief Assign a role to the newly spawned player
     */
    void assignRoleToNewPlayer();

    /**
     * @brief Determine which role is most needed (always harvester)
     * @return string representing the needed role
     */
    std::string determineNeededRole() const;

    /// Cooldown between spawns
    int spawnCooldown;

    /// Counter for checking resources
    int resourceCheckCounter;

    /// Whether a player was recently spawned
    bool recentlySpawned;
};

#endif  // ZAPPY_AI_SRC_MODULES_RESSOURCEGATHERINGSPAWNING_HPP_
