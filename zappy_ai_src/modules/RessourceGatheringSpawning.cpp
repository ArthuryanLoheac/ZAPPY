/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Resource Gathering and Spawning Module Implementation
*/

#include "modules/RessourceGatheringSpawning.hpp"
#include <iostream>
#include <chrono>
#include <unistd.h>
#include "../Interface/Interface.hpp"
#include "../Data/Data.hpp"

/**
 * @brief Initialize the RessourceGatheringSpawning with default values
 */
RessourceGatheringSpawning::RessourceGatheringSpawning()
    : spawnCooldown(0), resourceCheckCounter(0), recentlySpawned(false),
      needToFeed(false), foodCollectionCounter(0), spawnedRole("") {
    std::cout << "Resource Gathering & Spawning Module initialized" << std::endl;
}

/**
 * @brief Execute the spawning behavior
 */
void RessourceGatheringSpawning::execute() {
    // Handle food collection from feeder if needed
    if (needToFeed) {
        collectFoodFromFeeder();
        return;
    }

    // Decrement cooldown if active
    if (spawnCooldown > 0) {
        spawnCooldown--;
        return;
    }

    // Check inventory periodically
    if (++resourceCheckCounter >= 15) {
        resourceCheckCounter = 0;
        AI::Interface::i().sendCommand(INVENTORY);
        return;
    }

    // If we just spawned a player, assign it a role
    if (recentlySpawned) {
        assignRoleToNewPlayer();
        recentlySpawned = false;
        
        // If we spawned a feeder, prepare to collect food
        if (spawnedRole == "NEED_FEEDER") {
            needToFeed = true;
            foodCollectionCounter = 0;
        } else {
            spawnCooldown = 0; // Set cooldown before next spawn attempt
        }
        return;
    }

    // Check if we have enough resources to spawn
    if (hasSufficientResources()) {
        std::cout << "Spawning new player with FORK command..." << std::endl;
        AI::Interface::i().sendCommand(FORK);
        recentlySpawned = true;
        spawnCooldown = 0; // Short cooldown to allow time for spawn to complete
    }
}

/**
 * @brief Calculate the priority of this module
 * @return float Priority value between 0.0 and 1.0
 */
float RessourceGatheringSpawning::getPriority() {
    // Highest priority when we need to collect food from feeder
    if (needToFeed) {
        return 0.05f;
    }
    
    // Higher priority when we need to assign a role to a newly spawned player
    if (recentlySpawned) {
        return 0.1f;
    }
    
    // Medium priority when we have enough resources to spawn
    if (hasSufficientResources()) {
        std::cout << "Sufficient resources for spawning, priority: 0.3" << std::endl;
        return 0.3f;
    }
    
    // Low priority otherwise
    return 0.8f;
}

/**
 * @brief Check if we have enough resources to spawn
 * @return true if we have sufficient resources
 */
bool RessourceGatheringSpawning::hasSufficientResources() const {
    // Check if we have enough food to support spawning
    // (need at least 2 food to safely spawn)
    const int foodThreshold = 2;
    
    int foodCount = AI::Data::i().inventory.find("food") !=
                    AI::Data::i().inventory.end() ?
                    AI::Data::i().inventory.at("food") : 0;
    
    return foodCount >= foodThreshold && 
           spawnCooldown == 0 && 
           !recentlySpawned &&
           !needToFeed;
}

/**
 * @brief Determine which role is needed
 * @return string representing the needed role
 */
std::string RessourceGatheringSpawning::determineNeededRole() const {
    // Check current food level
    int foodCount = AI::Data::i().inventory.find("food") !=
                   AI::Data::i().inventory.end() ?
                   AI::Data::i().inventory.at("food") : 0;
    
    // If food is low, prioritize spawning a feeder
    if (foodCount < 5) {
        std::cout << "Food level low (" << foodCount << "), spawning a feeder" << std::endl;
        return "NEED_FEEDER";
    }
    
    // Otherwise spawn a harvester
    std::cout << "Food level adequate (" << foodCount << "), spawning a harvester" << std::endl;
    return "NEED_HARVESTER";
}

/**
 * @brief Assign a role to the newly spawned player
 */
void RessourceGatheringSpawning::assignRoleToNewPlayer() {
    spawnedRole = determineNeededRole();
    std::cout << "Assigning role to new player: " << spawnedRole << std::endl;
    
    // Broadcast the role assignment to the newly spawned player
    for (int i = 0; i < 5; ++i) {
        AI::Interface::i().sendMessage(spawnedRole);
    }
}

/**
 * @brief Handle the food collection from a feeder
 */
void RessourceGatheringSpawning::collectFoodFromFeeder() {
    if (foodCollectionCounter < 8) {
        std::cout << "Collecting food from feeder (" << (foodCollectionCounter + 1) 
                  << "/8)" << std::endl;
        AI::Interface::i().sendCommand("Take food\n");
        foodCollectionCounter++;
    } else {
        std::cout << "Finished collecting food from feeder" << std::endl;
        needToFeed = false;
        spawnCooldown = 50; // Set cooldown before next spawn attempt
    }
}
