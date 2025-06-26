/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Disruption Module Implementation
*/

#include "modules/DisruptionModule.hpp"
#include <iostream>
#include <string>
#include "../Interface/Interface.hpp"
#include "../Data/Data.hpp"

/**
 * @brief Initialize the DisruptionModule with default values
 */
DisruptionModule::DisruptionModule()
    : lookProbability(0), actionCounter(0), rng(std::random_device {}()) {
    std::cout << "Disruption Module initialized with "
              << "look probability: " << lookProbability << "%" << std::endl;
}

/**
 * @brief Execute the disruption behavior
 *
 * Sends random movement commands and occasionally sends LOOK commands
 * based on the current probability.
 */
void DisruptionModule::execute() {
    // Increment action counter
    actionCounter++;

    // Send random movement
    sendRandomMovement();

    // Check if we should send a LOOK command
    if (shouldSendLook()) {
        std::cout << "Disruption Module sending LOOK command..." << std::endl;
        AI::Interface::i().sendCommand(LOOK);

        // Update probability after sending LOOK
        updateLookProbability();
    }

    // Update probability every 5 actions regardless of whether LOOK was sent
    if (actionCounter % 5 == 0) {
        updateLookProbability();
    }
}

/**
 * @brief Send an attack command to a specific tile
 * @param x The x coordinate of the tile (forward steps from current position)
 * @param relativeY The relative y coordinate of the tile (positive values mean right, negative mean left; coordinates are relative to the player's current position)
 */
void DisruptionModule::attack(size_t x, int relativeY) {
    for (size_t i = 0; i < x; ++i) {
        AI::Interface::i().sendCommand(FORWARD);
    }
    if (relativeY != 0) {
        std::string direction = (relativeY > 0) ? RIGHT : LEFT;
        AI::Interface::i().sendCommand(direction);
        for (int i = 0; i < std::abs(relativeY); ++i) {
            AI::Interface::i().sendCommand(FORWARD);
        }
    }
    AI::Interface::i().sendCommand(EJECT);
}

/**
 * @brief Get the number of stones in a tile
 * @param tile The tile to check
 * @return int Number of stones in the tile
 */
int DisruptionModule::getNumberStone(std::unordered_map<std::string, int>
    &tile) {
    int numberStone = 0;

    for (const auto &item : tile) {
        if (item.first != "food" && item.first != "player") {
            numberStone += item.second;
        }
    }
    return numberStone;
}

/**
 * @brief Check if there is an elevation nearby
 * if there is one send the number of the case of the elevation
 */
int DisruptionModule::checkVisionElevation() {
    for (size_t x = 0; x < AI::Data::i().vision.size(); x++) {
        const size_t midY = AI::Data::i().vision[x].size() / 2;
        for (size_t y = 0; y < AI::Data::i().vision[x].size(); y++) {
            int relativeY = static_cast<int>(y) - static_cast<int>(midY);

            auto& tile = AI::Data::i().vision[x][y];

            if (tile["player"] > 2 && getNumberStone(tile) > 2) {
                std::cout << "Disruption Module found elevation at: "
                            << x << ", " << relativeY << std::endl;
                attack(x, relativeY);
                return 1;
            }
        }
    }
    return 0;
}

/**
 * @brief Calculate the priority of this module
 *
 * @return float Priority value between 0.0 and 1.0
 */
float DisruptionModule::getPriority() {
    // Lower priority than crucial modules like food gathering
    return 0.7f;
}

/**
 * @brief Send a random movement command
 */
void DisruptionModule::sendRandomMovement() {
    std::uniform_int_distribution<> distribution(1, 3);
    int randomChoice = distribution(rng);

    switch (randomChoice) {
        case 1:
            std::cout <<
            "Disruption Module sending FORWARD command" << std::endl;
            AI::Interface::i().sendCommand(FORWARD);
            break;
        case 2:
            std::cout << "Disruption Module sending LEFT command" << std::endl;
            AI::Interface::i().sendCommand(LEFT);
            break;
        case 3:
            std::cout << "Disruption Module sending RIGHT command" << std::endl;
            AI::Interface::i().sendCommand(RIGHT);
            break;
    }
}

/**
 * @brief Check if a LOOK command should be sent
 * @return true if a LOOK command should be sent
 */
bool DisruptionModule::shouldSendLook() {
    // If probability is 100%, always send LOOK
    if (lookProbability == 100) {
        return true;
    }

    // Otherwise, roll a random number between 1 and 100
    std::uniform_int_distribution<> distribution(1, 100);
    int roll = distribution(rng);

    // Return true if roll is less than or equal to the current probability
    return roll <= lookProbability;
}

/**
 * @brief Update the LOOK probability
 */
void DisruptionModule::updateLookProbability() {
    // If at 100%, reset to 0
    if (lookProbability == 100) {
        lookProbability = 0;
    } else {
        // Otherwise, increase by 40%
        lookProbability += 40;
    }

    // Ensure probability doesn't exceed 100%
    if (lookProbability > 100) {
        lookProbability = 100;
    }

    std::cout << "Disruption Module updated look probability to: "
              << lookProbability << "%" << std::endl;
}
