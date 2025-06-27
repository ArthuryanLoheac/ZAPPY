#include "modules/KirbyModule.hpp"

#include <unistd.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <algorithm>

#include "Interface/Interface.hpp"
#include "Data/Data.hpp"
#include "include/logs.h"
#include "../../libc/include/logs.h"

/**
 * @brief Constructs a new KirbyModule object.
 */
KirbyModule::KirbyModule() : tickUsed(0), timeRemaining(FOOD_TICK * 10),
    forwardCount(0), suckMode(true), hasMadeHisWill(false) {
    std::cout << "KirbyModule initialized: suckMode=" << suckMode
              << ", timeRemaining=" << timeRemaining << std::endl;
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
    std::cout << "Kirby PID " << getpid() << " executing with "
              << timeRemaining - tickUsed << " ticks remaining, "
              << (suckMode ? "SUCK mode" : "SPIT mode") << std::endl;
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
    return 0.1f;
}

/**
 * @brief Computes whether the KirbyModule should remain in suck mode.
 *
 * Updates the suckMode flag based on time, map size, and actions left.
 */
void KirbyModule::computeSuckMode() {
    int mapX = AI::Data::i().mapX;
    int mapY = AI::Data::i().mapY;
    int ticksForRotation = 2 * 7;
    int ticksForReturn = forwardCount * 7;
    int ticksForDropping = getNbObjects() * 7;
    int ticksNeededToReturnByTurning = ticksForRotation + ticksForReturn
        + ticksForDropping;
    int ticksRemaining = timeRemaining - tickUsed;
    const int SAFETY_MARGIN = 35;
    bool canLoopAround = false;
    int ticksNeededToReturnByLooping = 0;

    if (mapX > 0 && mapY > 0) {
        bool isSquareMap = (mapX == mapY);
        if (isSquareMap) {
            int stepsToLoopAround = mapX - forwardCount;
            ticksNeededToReturnByLooping =
                (stepsToLoopAround * 7) + ticksForDropping;
            canLoopAround =
                (ticksNeededToReturnByLooping < ticksNeededToReturnByTurning);
            std::cout << "Map is " << mapX << "x" << mapY << " (square). ";
            std::cout << "Steps taken: " << forwardCount << ". ";
            std::cout << "Ticks to loop around: " <<
                ticksNeededToReturnByLooping << ". ";
            std::cout << "Ticks to turn back: " <<
                ticksNeededToReturnByTurning << std::endl;
            if (forwardCount >= mapX) {
                std::cout << "Reached full map circuit, stopping to drop items"
                    << std::endl;
                suckMode = false;
                shouldLoopAround = (ticksNeededToReturnByLooping <
                    ticksNeededToReturnByTurning);
                hasMadeHisWill = false;
                return;
            }
        } else {
            int longestDimension = std::max(mapX, mapY);
            if (forwardCount >= longestDimension) {
                std::cout << "Reached longest dimension (" << longestDimension
                          << "), turning back" << std::endl;
                suckMode = false;
                shouldLoopAround = false;
                hasMadeHisWill = false;
                return;
            }
        }
    }
    int ticksNeededToReturn = canLoopAround ?
                             ticksNeededToReturnByLooping :
                             ticksNeededToReturnByTurning;
    if (ticksRemaining - ticksNeededToReturn <= SAFETY_MARGIN) {
        if (suckMode) {
            LOG_INFO("Kirby PID %i switching to SPIT mode (ticks remaining: %i"
                    ", ticks needed: %i, safety margin: %i)\n",
                    getpid(), ticksRemaining, ticksNeededToReturn,
                        SAFETY_MARGIN);
            hasMadeHisWill = false;
            suckMode = false;
            shouldLoopAround = canLoopAround;
        }
    }
}

/**
 * @brief Performs the "suck" action: looks, takes objects, and moves forward.
 */
void KirbyModule::suck() {
    computeSuckMode();
    if (!suckMode)
        return;
    AI::Interface::i().sendCommand(LOOK);
    tickUsed += 7;
    takeObjects();
    AI::Interface::i().sendCommand(FORWARD);
    tickUsed += 7;
    forwardCount++;
    std::cout << "Kirby moved forward, now at step " << forwardCount
              << ", used " << tickUsed << " ticks" << std::endl;
}

/**
 * @brief Returns to base and drops collected objects.
 */
void KirbyModule::spit() {
    if (hasMadeHisWill) {
        return;
    }
    hasMadeHisWill = true;
    int mapX = AI::Data::i().mapX;
    int mapY = AI::Data::i().mapY;
    bool isSquareMap = (mapX == mapY) && (mapX > 0) && (mapY > 0);
    int objectsToDrop = getNbObjects();
    std::cout << "Kirby returning to drop " << objectsToDrop
              << " objects after moving " << forwardCount << " steps"
              << std::endl;
    std::cout << "Map size: " << mapX << "x" << mapY
              << ", is square: " << (isSquareMap ? "yes" : "no")
              << ", Should loop around: " << (shouldLoopAround ? "yes" : "no")
              << std::endl;
    if (shouldLoopAround && isSquareMap && forwardCount <= mapX) {
        int stepsToLoopAround = mapX - forwardCount;
        if (stepsToLoopAround == 0) {
            std::cout << "Back to starting position" << std::endl;
        } else {
            std::cout << "Looping around the map! Taking " << stepsToLoopAround
                << " more steps forward instead of turning back" << std::endl;
            for (int i = 0; i < stepsToLoopAround; i++) {
                AI::Interface::i().sendCommand(FORWARD);
                tickUsed += 7;
            }
        }
    } else {
        std::cout << "Turning around and going back " << forwardCount <<
            " steps" << std::endl;
        AI::Interface::i().sendCommand(LEFT);
        AI::Interface::i().sendCommand(LEFT);
        tickUsed += 14;
        for (int i = 0; i < forwardCount; i++) {
            AI::Interface::i().sendCommand(FORWARD);
            tickUsed += 7;
        }
    }
    int itemsDropped = 0;
    for (auto &item : AI::Data::i().inventory) {
        const std::string itemName = item.first;
        const int itemCount = item.second;
        for (int i = 0; i < itemCount; i++) {
            AI::Interface::i().sendCommand("Set " + itemName + "\n");
            tickUsed += 7;
            itemsDropped++;
        }
    }
    std::cout << "Kirby completed trip: walked " << forwardCount
              << " steps, dropped " << itemsDropped << " items, used "
              << tickUsed << " ticks out of " << timeRemaining << std::endl;
}

/**
 * @brief Takes all objects from the cell in front of the agent.
 */
void KirbyModule::takeObjects() {
    try {
        auto &cell = AI::Data::i().mapAt(1, 0);
        int itemsTaken = 0;

        for (auto &item : cell) {
            const std::string itemName = item.first;
            const int itemCount = item.second;
            if (itemName == "food") {
                continue;
            }
            for (int i = 0; i < itemCount; i++) {
                AI::Interface::i().sendCommand("Take " + itemName + "\n");
                tickUsed += 7;
                itemsTaken++;
            }
        }
        if (cell.find("food") != cell.end() && cell["food"] > 0) {
            for (int i = 0; i < cell["food"]; i++) {
                AI::Interface::i().sendCommand("Take food\n");
                tickUsed += 7;
                itemsTaken++;
            }
        }
        if (itemsTaken > 0) {
            std::cout << "Collected " << itemsTaken << " items" << std::endl;
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
        // Don't count food when calculating objects to drop
        if (item.first != "food") {
            nbObjects += item.second;
        }
    }
    return nbObjects;
}
