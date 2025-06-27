/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Resource Gathering and Spawning Module
*/

#ifndef ZAPPY_AI_SRC_MODULES_RESSOURCEGATHERINGSPAWNING_HPP_
#define ZAPPY_AI_SRC_MODULES_RESSOURCEGATHERINGSPAWNING_HPP_

#include <string>
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
     * @brief Handle the food collection from a feeder
     */
    void collectFoodFromFeeder();

    /**
     * @brief Determine which role is needed
     * @return string representing the needed role
     */
    std::string determineNeededRole() const;

    /**
     * @brief Take all objects from the current tile
     * @return Number of objects taken
     */
    int takeAllObjects();

    /**
     * @brief Check for and take all objects on current tile
     * @return True if any objects were taken
     */
    bool checkAndTakeObjects();

    int spawnCooldown;

    int resourceCheckCounter;

    bool recentlySpawned;

    bool needToFeed;

    int foodCollectionCounter;

    std::string spawnedRole;

    int harvesterCount;

    int feedLookCounter;

    bool emergencyLowFood;

    bool creatingEmergencyFeeder;

    bool waitingForEmergencyFood;
};

#endif  // ZAPPY_AI_SRC_MODULES_RESSOURCEGATHERINGSPAWNING_HPP_
