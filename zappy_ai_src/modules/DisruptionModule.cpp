/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Disruption Module Implementation
*/

#include "modules/DisruptionModule.hpp"
#include <iostream>
#include "../Interface/Interface.hpp"
#include "../Data/Data.hpp"
#include "../../libc/include/logs.h"

/**
 * @brief Initialize the DisruptionModule with default values
 */
DisruptionModule::DisruptionModule(): lookProbability(0), actionCounter(0),
    rng(std::random_device {} ()) {
    LOG_INFO("Disruption Module initialized with look probability: %d%%",
        lookProbability);
}

/**
 * @brief Execute the disruption behavior
 *
 * Sends random movement commands and occasionally sends LOOK commands
 * based on the current probability.
 */
void DisruptionModule::execute() {
    actionCounter++;
    sendRandomMovement();
    if (shouldSendLook()) {
        LOG_INFO("Disruption Module sending LOOK command...");
        AI::Interface::i().sendCommand(LOOK);
        updateLookProbability();
    }
    if (actionCounter % 5 == 0) {
        updateLookProbability();
    }
}

/**
 * @brief Calculate the priority of this module
 *
 * @return float Priority value between 0.0 and 1.0
 */
float DisruptionModule::getPriority() {
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
            LOG_INFO("Disruption Module sending FORWARD command");
            AI::Interface::i().sendCommand(FORWARD);
            break;
        case 2:
            LOG_INFO("Disruption Module sending LEFT command");
            AI::Interface::i().sendCommand(LEFT);
            break;
        case 3:
            LOG_INFO("Disruption Module sending RIGHT command");
            AI::Interface::i().sendCommand(RIGHT);
            break;
    }
}

/**
 * @brief Check if a LOOK command should be sent
 * @return true if a LOOK command should be sent
 */
bool DisruptionModule::shouldSendLook() {
    if (lookProbability == 100) {
        return true;
    }
    std::uniform_int_distribution<> distribution(1, 100);
    int roll = distribution(rng);
    return roll <= lookProbability;
}

/**
 * @brief Update the LOOK probability
 */
void DisruptionModule::updateLookProbability() {
    if (lookProbability == 100) {
        lookProbability = 0;
    } else {
        lookProbability += 40;
    }
    if (lookProbability > 100) {
        lookProbability = 100;
    }
    LOG_INFO("Disruption Module updated look probability to: %d%%",
             lookProbability);
}
