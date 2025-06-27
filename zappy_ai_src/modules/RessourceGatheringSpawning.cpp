/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Resource Gathering and Spawning Module Implementation
*/

#include <unistd.h>
#include <string>
#include <iostream>
#include <chrono>
#include "modules/RessourceGatheringSpawning.hpp"
#include "../Interface/Interface.hpp"
#include "../Data/Data.hpp"

/**
 * @brief Initialize the RessourceGatheringSpawning with default values
 */
RessourceGatheringSpawning::RessourceGatheringSpawning()
    : spawnCooldown(0), resourceCheckCounter(0), recentlySpawned(false),
      needToFeed(false), foodCollectionCounter(0), spawnedRole(""),
      harvesterCount(0), feedLookCounter(0), emergencyLowFood(false),
      creatingEmergencyFeeder(false), waitingForEmergencyFood(false) {
    std::cout << "Resource GatheringSpawning Module initialized" << std::endl;
}

/**
 * @brief Execute the spawning behavior
 */
void RessourceGatheringSpawning::execute() {
    int foodCount = AI::Data::i().inventory.find("food") !=
                    AI::Data::i().inventory.end() ?
                    AI::Data::i().inventory.at("food") : 0;
    emergencyLowFood = (foodCount < 3);
    if (emergencyLowFood && foodCount >= 2 && !creatingEmergencyFeeder &&
        !waitingForEmergencyFood) {
        std::cout << "EMERGENCY: Low food detected (" << foodCount << ")."
            " Creating emergency feeder!" << std::endl;
        AI::Interface::i().sendCommand(FORK);
        creatingEmergencyFeeder = true;
        return;
    }
    if (creatingEmergencyFeeder && !recentlySpawned) {
        std::cout << "EMERGENCY: Assigning FEEDER role to emergency AI"
            << std::endl;
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
                std::cout << "Collected items from tile during feeding"
                    << std::endl;
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
        std::cout << "Spawning new player with FORK command..." << std::endl;
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
    int foodCount = AI::Data::i().inventory.find("food") !=
                    AI::Data::i().inventory.end() ?
                    AI::Data::i().inventory.at("food") : 0;
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
        std::cout << "Sufficient resources for spawning, priority: 0.3"
            << std::endl;
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
    int foodCount = AI::Data::i().inventory.find("food") !=
                   AI::Data::i().inventory.end() ?
                   AI::Data::i().inventory.at("food") : 0;
    if (harvesterCount >= 2) {
        std::cout << "Already spawned " << harvesterCount
                 << " harvesters, spawning a feeder" << std::endl;
        return "NEED_FEEDER";
    }
    if (foodCount < 5) {
        std::cout << "Food level low (" << foodCount << "), spawning a feeder"
            << std::endl;
        return "NEED_FEEDER";
    }
    std::cout << "Food level adequate (" << foodCount << "),"
        " spawning a harvester" << std::endl;
    return "NEED_HARVESTER";
}

/**
 * @brief Assign a role to the newly spawned player
 */
void RessourceGatheringSpawning::assignRoleToNewPlayer() {
    spawnedRole = determineNeededRole();
    std::cout << "Assigning role to new player: " << spawnedRole << std::endl;
    if (spawnedRole == "NEED_HARVESTER") {
        harvesterCount++;
        std::cout << "Harvester count is now: " << harvesterCount << std::endl;
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
        std::cout << "Collecting food from feeder ("
                  << (foodCollectionCounter + 1) << "/10)" << std::endl;
        AI::Interface::i().sendCommand("Take food\n");
        foodCollectionCounter++;
    } else {
        std::cout << "Finished collecting food from feeder" << std::endl;
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
        std::cout << "Error taking objects: " << e.what() << std::endl;
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
        std::cout << "Collected " << itemsTaken << " items" << std::endl;
        return true;
    }
    return false;
}
