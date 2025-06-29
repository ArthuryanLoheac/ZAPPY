#include "modules/KirbyModule.hpp"

#include <unistd.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iostream>

#include "Interface/Interface.hpp"
#include "Data/Data.hpp"
#include "include/logs.h"
#include "../../libc/include/logs.h"

/**
 * @brief Constructs a new KirbyModule object.
 * 
 * Initializes the module in suck mode with a time budget of FOOD_TICK * 10.
 */
KirbyModule::KirbyModule() : tickUsed(0), timeRemaining(FOOD_TICK * 10),
    forwardCount(0), suckMode(true), hasMadeHisWill(false),
    shouldLoopAround(false) {
    LOG_INFO("KirbyModule initialized: suckMode=%s, timeRemaining=%d",
             (suckMode ? "true" : "false"), timeRemaining);
}

/**
 * @brief Destroys the KirbyModule object.
 */
KirbyModule::~KirbyModule() = default;

/**
 * @brief Executes the main logic of the KirbyModule.
 *
 * Decides to perform the suck or spit action based on the current mode.
 */
void KirbyModule::execute() {
    LOG_INFO("Kirby PID %d executing with %d ticks remaining, %s mode",
             getpid(), timeRemaining - tickUsed, (suckMode ? "SUCK" : "SPIT"));
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
 * Updates the suckMode flag based on remaining time, map dimensions, and
 * efficiency calculations. Switches to spit mode when approaching time limits
 * or map boundaries. For square maps, determines if looping around is more
 * efficient than turning back.
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
            LOG_INFO("Map is %dx%d (square). Steps taken: %d. Ticks to loop"
                " around: %d. Ticks to turn back: %d",
                mapX, mapY, forwardCount, ticksNeededToReturnByLooping,
                ticksNeededToReturnByTurning);
            if (forwardCount >= mapX) {
                LOG_INFO("Reached full map circuit, stopping to drop items");
                suckMode = false;
                shouldLoopAround = (ticksNeededToReturnByLooping <
                    ticksNeededToReturnByTurning);
                hasMadeHisWill = false;
                return;
            }
        } else {
            int longestDimension = std::max(mapX, mapY);
            if (forwardCount >= longestDimension) {
                LOG_INFO("Reached longest dimension (%d), turning back",
                    longestDimension);
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
 * @brief Performs the "suck" action: looks around, takes objects, and moves
 * forward.
 * 
 * First checks if mode should switch, then executes look, take, and forward
 * commands.
 * Increments step counter and updates tick usage.
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
    LOG_INFO("Kirby moved forward, now at step %d, used %d ticks",
        forwardCount, tickUsed);
}

/**
 * @brief Move the player back to the starting position.
 * Handles both looping around and turning back.
 */
void KirbyModule::moveBackToStart() {
    int mapX = AI::Data::i().mapX;
    int mapY = AI::Data::i().mapY;
    bool isSquareMap = (mapX == mapY) && (mapX > 0) && (mapY > 0);

    if (shouldLoopAround && isSquareMap && forwardCount <= mapX) {
        int stepsToLoopAround = mapX - forwardCount;
        if (stepsToLoopAround == 0) {
            LOG_INFO("Back to starting position");
        } else {
            LOG_INFO("Looping around the map! Taking %d more steps forward"
                " instead of turning back", stepsToLoopAround);
            for (int i = 0; i < stepsToLoopAround; i++) {
                AI::Interface::i().sendCommand(FORWARD);
                tickUsed += 7;
            }
        }
    } else {
        LOG_INFO("Turning around and going back %d steps", forwardCount);
        AI::Interface::i().sendCommand(LEFT);
        AI::Interface::i().sendCommand(LEFT);
        tickUsed += 14;
        for (int i = 0; i < forwardCount; i++) {
            AI::Interface::i().sendCommand(FORWARD);
            tickUsed += 7;
        }
    }
}

/**
 * @brief Drop all items in inventory, non-food first, then food.
 * @return Number of items dropped.
 */
int KirbyModule::dropAllInventory() {
    int itemsDropped = 0;
    for (auto &item : AI::Data::i().inventory) {
        if (item.first == AI::Data::Material_t::Food)
            continue;
        std::string itemName = AI::Data::materialToString(item.first);
        std::transform(itemName.begin(), itemName.end(), itemName.begin(),
            ::tolower);
        const int itemCount = item.second;
        for (int i = 0; i < itemCount; i++) {
            AI::Interface::i().sendCommand("Set " + itemName + "\n");
            tickUsed += 7;
            itemsDropped++;
        }
    }
    for (auto &item : AI::Data::i().inventory) {
        if (item.first != AI::Data::Material_t::Food)
            continue;
        std::string itemName = AI::Data::materialToString(item.first);
        std::transform(itemName.begin(), itemName.end(), itemName.begin(),
            ::tolower);
        const int itemCount = item.second;
        for (int i = 0; i < itemCount; i++) {
            AI::Interface::i().sendCommand("Set " + itemName + "\n");
            tickUsed += 7;
            itemsDropped++;
        }
    }
    return itemsDropped;
}

/**
 * @brief Returns to starting position and drops all collected objects.
 * 
 * Chooses optimal return strategy based on map geometry and efficiency:
 * - For square maps: may loop around if more efficient than turning back
 * - Otherwise: turns around and retraces steps
 * Drops all non-food items from inventory at the starting position.
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
    LOG_INFO("Kirby returning to drop %d objects after moving %d steps",
            objectsToDrop, forwardCount);
    LOG_INFO("Map size: %dx%d, is square: %s, Should loop around: %s", mapX,
        mapY, (isSquareMap ? "yes" : "no"), (shouldLoopAround ? "yes" : "no"));

    moveBackToStart();
    int itemsDropped = dropAllInventory();

    AI::Interface::i().sendCommand("Inventory\n");
    int remaining = 0;
    for (const auto& item : AI::Data::i().inventory) {
        if (item.second > 0)
            remaining += item.second;
    }
    if (remaining > 0) {
        LOG_INFO("Redundancy: Inventory not empty after drop, dropping again");
        itemsDropped += dropAllInventory();
    }

    LOG_INFO("Kirby completed trip: walked %d steps, dropped %d items,"
        " used %d ticks out of %d",
             forwardCount, itemsDropped, tickUsed, timeRemaining);
}

/**
 * @brief Takes all objects from the current cell after a look command.
 * 
 * Prioritizes collection of non-food items first, then collects food.
 * Updates tick usage for each take command executed.
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
            LOG_INFO("Collected %d items", itemsTaken);
        }
    } catch (const std::out_of_range &e) {
        LOG_WARNING("Error taking objects: %s", e.what());
    }
}

/**
 * @brief Gets the total number of non-food objects in the inventory.
 * @return The number of objects as an integer (food is excluded from count).
 */
int KirbyModule::getNbObjects() {
    int nbObjects = 0;

    for (auto &item : AI::Data::i().inventory) {
        // Don't count food when calculating objects to drop
        if (item.first != AI::Data::Material_t::Food) {
            nbObjects += item.second;
        }
    }
    return nbObjects;
}
