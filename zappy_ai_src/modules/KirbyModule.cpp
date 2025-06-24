#include "modules/KirbyModule.hpp"

#include <unistd.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>

#include "Interface/Interface.hpp"
#include "Data/Data.hpp"
#include "include/logs.h"

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
 * Updates the suckMode flag based on time and actions left.
 */
void KirbyModule::computeSuckMode() {
    // Calculate ticks needed to return and drop items
    int ticksForRotation = 2 * 7;  // 180 degrees turn (LEFT twice)
    int ticksForReturn = forwardCount * 7;  // Each step back costs 7 ticks
    int ticksForDropping = getNbObjects() * 7;  // Each item drop costs 7 ticks
    
    int ticksNeededToReturn = ticksForRotation + ticksForReturn + ticksForDropping;
    int ticksRemaining = timeRemaining - tickUsed;
    
    const int SAFETY_MARGIN = 35; // Safety margin in ticks
    
    if (ticksRemaining - ticksNeededToReturn <= SAFETY_MARGIN) {
        if (suckMode) {
            LOG_INFO("Kirby PID %i switching to SPIT mode (ticks remaining: %i, " 
                    "ticks needed: %i, safety margin: %i)\n", 
                    getpid(), ticksRemaining, ticksNeededToReturn, SAFETY_MARGIN);
            suckMode = false;
        }
    }
}

/**
 * @brief Performs the "suck" action: looks, takes objects, and moves forward.
 */
void KirbyModule::suck() {
    // Check if we should switch to spit mode
    computeSuckMode();
    
    // If we've switched to spit mode, return immediately
    if (!suckMode) {
        return;
    }
    
    // Look for objects
    AI::Interface::i().sendCommand(LOOK);
    tickUsed += 7;

    // Take objects from the current location
    takeObjects();

    // Move forward
    AI::Interface::i().sendCommand(FORWARD);
    tickUsed += 7;
    forwardCount++;
    
    std::cout << "Kirby moved forward, now at step " << forwardCount 
              << ", used " << tickUsed << " ticks" << std::endl;
}

/**
 * @brief Performs the "spit" action: retraces steps and drops collected objects.
 */
void KirbyModule::spit() {
    if (hasMadeHisWill) {
        return;
    }

    hasMadeHisWill = true;
    std::cout << "Kirby returning home to drop " << getNbObjects() 
              << " objects after moving " << forwardCount << " steps" << std::endl;

    // Turn around (180 degrees)
    AI::Interface::i().sendCommand(LEFT);
    AI::Interface::i().sendCommand(LEFT);
    tickUsed += 14;  // 7 ticks per turn

    // Return to base (walk backward)
    for (int i = 0; i < forwardCount; i++) {
        AI::Interface::i().sendCommand(FORWARD);
        tickUsed += 7;
    }

    // Drop all items
    int itemsDropped = 0;
    for (auto &item : AI::Data::i().inventory) {
        const std::string itemName = item.first;
        const int itemCount = item.second;
        
        // Skip food items
        if (itemName == "food") {
            continue;
        }
        
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
            
            // Skip food for now - we want to prioritize valuable resources
            if (itemName == "food") {
                continue;
            }
            
            for (int i = 0; i < itemCount; i++) {
                AI::Interface::i().sendCommand("Take " + itemName + "\n");
                tickUsed += 7;
                itemsTaken++;
            }
        }
        
        // Now collect food if available
        if (cell.find("food") != cell.end() && cell["food"] > 0) {
            for (int i = 0; i < cell["food"]; i++) {
                AI::Interface::i().sendCommand("Take food\n");
                tickUsed += 7;
                itemsTaken++;
            }
        }
        
        if (itemsTaken > 0) {
            std::cout << "Kirby collected " << itemsTaken << " items" << std::endl;
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
