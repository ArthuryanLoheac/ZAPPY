#include "modules/KirbyModule.hpp"

#include <unistd.h>
#include <string>
#include <vector>
#include <stdexcept>

#include "Interface/Interface.hpp"
#include "Data/Data.hpp"
#include "include/logs.h"

/**
 * @brief Constructs a new KirbyModule object.
 */
KirbyModule::KirbyModule() : tickUsed(0), timeRemaining(FOOD_TICK * 10),
    forwardCount(0), suckMode(false), hasMadeHisWill(false) {
}

/**
 * @brief Destroys the KirbyModule object.
 */
KirbyModule::~KirbyModule() = default;

/**
 * @brief Executes the main logic of the KirbyModule.
 *
 * Decides whether to perform the suck or spit action based on the current mode.
 */
void KirbyModule::execute() {
    if (suckMode) {
        suck();
    } else {
        spit();
    }
}

/**
 * @brief Returns the priority of this module.
 * @return The priority as a float value.
 */
float KirbyModule::getPriority() {
    return 1.0f;
}

/**
 * @brief Computes whether the KirbyModule should remain in suck mode.
 *
 * Updates the suckMode flag based on time and actions left.
 */
void KirbyModule::computeSuckMode() {
    int tickToDo = ((2 * 7)  // rotation of 180 degrees
        + (forwardCount * 7)  // walk back
        + (getNbObjects() * 7));  // objects to drop


    if (timeRemaining - tickUsed - tickToDo <= 35) {
        LOG_INFO("Kirby PID %i stops sucking\n", getpid());
        suckMode = false;
    }
}

/**
 * @brief Performs the "suck" action: looks, takes objects, and moves forward.
 */
void KirbyModule::suck() {
    AI::Interface::i().sendCommand(LOOK);
    tickUsed += 7;

    takeObjects();

    AI::Interface::i().sendCommand(FORWARD);
    tickUsed += 7;
    forwardCount++;
}

/**
 * @brief Performs the "spit" action: retraces steps and drops collected objects.
 */
void KirbyModule::spit() {
    if (hasMadeHisWill) {
        return;
    }

    hasMadeHisWill = true;

    AI::Interface::i().sendCommand(LEFT);
    AI::Interface::i().sendCommand(LEFT);

    for (int i = 0; i < forwardCount; i++) {
        AI::Interface::i().sendCommand(FORWARD);
    }

    for (auto &item : AI::Data::i().inventory) {
        while (item.second > 0) {
            AI::Interface::i().sendCommand("Set " + item.first);
            item.second--;
        }
    }
}

/**
 * @brief Takes all objects from the cell in front of the agent.
 */
void KirbyModule::takeObjects() {
    try {
        auto &cell = AI::Data::i().mapAt(1, 0);

        for (auto &item : cell) {
            while (item.second > 0) {
                tickUsed += 7;
                AI::Interface::i().sendCommand("Take " + item.first);
                item.second--;
            }
        }
    } catch (const std::out_of_range &e) {
        LOG_WARNING("Error taking objects: %s\n", e.what());
    }
}

/**
 * @brief Gets the total number of objects in the inventory.
 * @return The number of objects as an integer.
 */
int KirbyModule::getNbObjects() {
    int nbObjects = 0;

    for (auto &item : AI::Data::i().inventory) {
        nbObjects += item.second;
    }
    return nbObjects;
}
