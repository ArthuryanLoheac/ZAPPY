/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Resource Gathering and Spawning Module Implementation
*/

#include <unistd.h>
#include <string>
#include <chrono>
#include "modules/RessourceGatheringSpawning.hpp"
#include "../Interface/Interface.hpp"
#include "../Data/Data.hpp"
#include "../../libc/include/logs.h"

/**
 * @brief Initialize the RessourceGatheringSpawning with default values
 */
RessourceGatheringSpawning::RessourceGatheringSpawning()
    : spawnCooldown(0), resourceCheckCounter(0), recentlySpawned(false),
      needToFeed(false), foodCollectionCounter(0), spawnedRole(""),
      harvesterCount(0), feedLookCounter(0), emergencyLowFood(false),
      creatingEmergencyFeeder(false), waitingForEmergencyFood(false) {
    LOG_INFO("Resource GatheringSpawning Module initialized");
}

/**
 * @brief Execute the spawning behavior
 */
void RessourceGatheringSpawning::execute() {
    int foodCount = AI::Data::i().inventory.find(AI::Data::Material_t::Food) !=
                    AI::Data::i().inventory.end() ?
                    AI::Data::i().inventory.at(AI::Data::Material_t::Food) : 0;
    emergencyLowFood = (foodCount < 3);
    if (emergencyLowFood && foodCount >= 2 && !creatingEmergencyFeeder &&
        !waitingForEmergencyFood) {
        LOG_INFO("EMERGENCY: Low food detected (%d)."
            " Creating emergency feeder!", foodCount);
        AI::Interface::i().sendCommand(FORK);
        creatingEmergencyFeeder = true;
        return;
    }
    if (creatingEmergencyFeeder && !recentlySpawned) {
        LOG_INFO("EMERGENCY: Assigning FEEDER role to emergency AI");
        spawnedRole = "NEED_FEEDER";
        for (int i = 0; i < 5; ++i) {
            AI::Interface::i().sendMessage(spawnedRole);
        }
        creatingEmergencyFeeder = false;
        waitingForEmergencyFood = true;
        needToFeed = true;
        foodCollectionCounter = 0;
        feedLookCounter = 0;
        return;
    }
    if (needToFeed) {
        if (++feedLookCounter >= 5) {
            feedLookCounter = 0;
            AI::Interface::i().sendCommand(LOOK);
            if (checkAndTakeObjects()) {
                LOG_INFO("Collected items from tile during feeding");
            }
            return;
        }
        collectFoodFromFeeder();
        return;
    }
    if (spawnCooldown > 0) {
        spawnCooldown--;
        return;
    }
    if (++resourceCheckCounter >= 15) {
        resourceCheckCounter = 0;
        AI::Interface::i().sendCommand(INVENTORY);
        return;
    }
    if (recentlySpawned) {
        assignRoleToNewPlayer();
        recentlySpawned = false;
        if (spawnedRole == "NEED_FEEDER") {
            needToFeed = true;
            foodCollectionCounter = 0;
            feedLookCounter = 0;
        } else {
            spawnCooldown = 0;
        }
        return;
    }
    if (hasSufficientResources()) {
        LOG_INFO("Spawning new player with FORK command...");
        AI::Interface::i().sendCommand(FORK);
        recentlySpawned = true;
        spawnCooldown = 0;
    }
}

/**
 * @brief Calculate the priority of this module
 * @return float Priority value between 0.0 and 1.0
 */
float RessourceGatheringSpawning::getPriority() {
    int foodCount = AI::Data::i().inventory.find(AI::Data::Material_t::Food) !=
                    AI::Data::i().inventory.end() ?
                    AI::Data::i().inventory.at(AI::Data::Material_t::Food) : 0;
    if (emergencyLowFood || creatingEmergencyFeeder) {
        return 0.01f;
    }
    if (waitingForEmergencyFood || needToFeed) {
        if (foodCount <= 3) {
            return 0.01f;
        } else {
            float dynamicPriority = 0.01f + ((foodCount - 3) * 0.01f);
            return dynamicPriority;
        }
    }
    if (recentlySpawned) {
        return 0.1f;
    }
    if (hasSufficientResources()) {
        LOG_INFO("Sufficient resources for spawning, priority: 0.3");
        return 0.3f;
    }
    return 0.8f;
}

/**
 * @brief Check if we have enough resources to spawn
 * @return true if we have sufficient resources
 */
bool RessourceGatheringSpawning::hasSufficientResources() const {
    const int foodThreshold = 2;
    int foodCount = AI::Data::i().inventory.find(AI::Data::Material_t::Food) !=
                    AI::Data::i().inventory.end() ?
                    AI::Data::i().inventory.at(AI::Data::Material_t::Food) : 0;
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
    int foodCount = AI::Data::i().inventory.find(AI::Data::Material_t::Food) !=
                   AI::Data::i().inventory.end() ?
                   AI::Data::i().inventory.at(AI::Data::Material_t::Food) : 0;
    if (harvesterCount >= 2) {
        LOG_INFO("Already spawned %d harvesters, spawning a feeder",
            harvesterCount);
        return "NEED_FEEDER";
    }
    if (foodCount < 5) {
        LOG_INFO("Food level low (%d), spawning a feeder", foodCount);
        return "NEED_FEEDER";
    }
    LOG_INFO("Food level adequate (%d), spawning a harvester", foodCount);
    return "NEED_HARVESTER";
}

/**
 * @brief Assign a role to the newly spawned player
 */
void RessourceGatheringSpawning::assignRoleToNewPlayer() {
    spawnedRole = determineNeededRole();
    LOG_INFO("Assigning role to new player: %s", spawnedRole.c_str());
    if (spawnedRole == "NEED_HARVESTER") {
        harvesterCount++;
        LOG_INFO("Harvester count is now: %d", harvesterCount);
    }
    for (int i = 0; i < 5; ++i) {
        AI::Interface::i().sendMessage(spawnedRole);
    }
}

/**
 * @brief Handle the food collection from a feeder
 */
void RessourceGatheringSpawning::collectFoodFromFeeder() {
    if (foodCollectionCounter < 10) {
        LOG_INFO("Collecting food from feeder (%d/10)",
            foodCollectionCounter + 1);
        AI::Interface::i().sendCommand("Take food\n");
        foodCollectionCounter++;
    } else {
        LOG_INFO("Finished collecting food from feeder");
        needToFeed = false;
        waitingForEmergencyFood = false;
        spawnCooldown = 50;
    }
}

/**
 * @brief Take all objects from the current tile
 * @return Number of objects taken
 */
int RessourceGatheringSpawning::takeAllObjects() {
    int itemsTaken = 0;
    if (AI::Data::i().vision.empty() || AI::Data::i().vision[0].empty())
        return 0;
    try {
        const size_t midY = AI::Data::i().vision[0].size() / 2;
        auto& currentTile = AI::Data::i().vision[0][midY];
        for (auto& item : currentTile) {
            const std::string& itemName = item.first;
            int itemCount = item.second;
            for (int i = 0; i < itemCount; i++) {
                AI::Interface::i().sendCommand("Take " + itemName + "\n");
                itemsTaken++;
            }
        }
    } catch (const std::exception& e) {
        LOG_INFO("Error taking objects: %s", e.what());
    }
    return itemsTaken;
}

/**
 * @brief Check for and take all objects on current tile
 * @return True if any objects were taken
 */
bool RessourceGatheringSpawning::checkAndTakeObjects() {
    int itemsTaken = takeAllObjects();
    if (itemsTaken > 0) {
        LOG_INFO("Collected %d items", itemsTaken);
        return true;
    }
    return false;
}
