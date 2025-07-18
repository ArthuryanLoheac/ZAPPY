/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Role Attribution Module Implementation
*/

#include <unistd.h>
#include <iostream>
#include <regex>
#include <random>
#include <chrono>
#include <string>
#include "modules/RoleAttributionModule.hpp"
#include "../Interface/Interface.hpp"
#include "../Data/Data.hpp"

/**
 * @brief Initialize the RoleAttributionModule with default values
 */
RoleAttributionModule::RoleAttributionModule()
    : currentRole(Role::UNKNOWN), currentPhase(AttributionPhase::INITIAL_LOOK),
      lookCounter(0), myId(-1),
      rng(static_cast<unsigned int>(getpid()) ^
          static_cast<unsigned int>(std::chrono::steady_clock::now().
          time_since_epoch().count())),
      idDistribution(1, 1000) {
    std::cout << "Role Attribution Module initialized with role: "
              << roleToString(currentRole) << std::endl;
    std::cout << "Starting initial LOOK phase with PID-based random seed: "
              << getpid() << std::endl;
}

/**
 * @brief Execute the role attribution behavior
 *
 * Manages the phases of role attribution and sends commands accordingly
 */
void RoleAttributionModule::execute() {
    processMessages();
    switch (currentPhase) {
        case AttributionPhase::INITIAL_LOOK:
            if (lookCounter < 3) {
                std::cout << "Role Attribution Module sending LOOK command "
                          << (lookCounter + 1) << "/3..." << std::endl;
                AI::Interface::i().sendCommand(LOOK);
                lookCounter++;
                if (lookCounter >= 3 && currentRole == Role::UNKNOWN) {
                    std::cout << "No role assigned from NEED messages. "
                              << "Moving to ID attribution phase..."
                              << std::endl;
                    currentPhase = AttributionPhase::BROADCASTING_ID;
                    AI::Interface::i().sendMessage("ID_ATTRIBUTION");
                    lookCounter = 0;
                }
            }
            break;

        case AttributionPhase::BROADCASTING_ID:
            currentPhase = AttributionPhase::COLLECTING_IDS;
            lookCounter = 0;
            break;
        case AttributionPhase::COLLECTING_IDS:
            if (lookCounter < 3) {
                std::cout << "Collecting IDs - LOOK command "
                          << (lookCounter + 1) << "/3... "
                          << time(nullptr) << std::endl;
                AI::Interface::i().sendCommand(LOOK);
                lookCounter++;
                if (lookCounter >= 3) {
                    myId = generateRandomId();
                    std::cout << "Assigning random ID: " << myId << std::endl;
                    assignRoleFromId();
                    AI::Interface::i().sendMessage("I_AM_"
                        + std::to_string(myId));
                    currentPhase = AttributionPhase::ROLE_ASSIGNED;
                }
            }
            break;

        case AttributionPhase::ROLE_ASSIGNED:
            break;
    }
}

/**
 * @brief Calculate the priority of this module
 *
 * Priority is highest during role attribution phases, lower once assigned.
 *
 * @return float Priority value between 0.0 and 1.0
 */
float RoleAttributionModule::getPriority() {
    return (currentPhase == AttributionPhase::ROLE_ASSIGNED) ? 1.0f : 0.3f;
}

/**
 * @brief Get the current role of the AI
 * @return Role The current role
 */
Role RoleAttributionModule::getCurrentRole() const {
    return currentRole;
}

/**
 * @brief Generate a random ID
 * @return int A random integer ID
 */
int RoleAttributionModule::generateRandomId() {
    return idDistribution(rng);
}

/**
 * @brief Assign role based on ID
 */
void RoleAttributionModule::assignRoleFromId() {
    currentRole = Role::LEVELER;
    currentPhase = AttributionPhase::ROLE_ASSIGNED;
    std::cout << "Role assigned based on random ID: "
              << roleToString(currentRole) << std::endl;
}

/**
 * @brief Reassign role using new random ID
 */
void RoleAttributionModule::reassignRole() {
    myId = generateRandomId();
    std::cout << "Reassigning with new random ID: " << myId << std::endl;
    assignRoleFromId();
    AI::Interface::i().sendMessage("NEW_ID_" + std::to_string(myId));
    std::cout << "Reassigned to: " << roleToString(currentRole) << std::endl;
}

/**
 * @brief Process incoming broadcast messages
 *
 * Routes messages to appropriate handlers based on current phase
 */
void RoleAttributionModule::processMessages() {
    while (!AI::Data::i().messageQueue.empty()) {
        auto message = AI::Data::i().messageQueue.front();
        AI::Data::i().messageQueue.pop();

        std::string content = message.first;
        int direction = message.second;

        std::cout << "Broadcast received - Content: '" << content
                  << "', Direction: " << direction << std::endl;

        if (content.find("REROLL") != std::string::npos &&
            currentPhase == AttributionPhase::ROLE_ASSIGNED) {
            std::cout << "REROLL, reassigning role..." << std::endl;
            reassignRole();
            continue;
        }
        switch (currentPhase) {
            case AttributionPhase::INITIAL_LOOK:
                processInitialPhase(content, direction);
                break;

            case AttributionPhase::COLLECTING_IDS:
            case AttributionPhase::BROADCASTING_ID:
                processIdCollectionPhase(content);
                break;

            case AttributionPhase::ROLE_ASSIGNED:
                break;
        }
    }
}

/**
 * @brief Process messages during the initial phase
 * @param content The message content
 * @param direction The message direction
 */
void RoleAttributionModule::processInitialPhase(
    const std::string& content, int direction) {
    if (direction != 0) {
        return;
    }

    bool roleAssigned = false;

    if (content.find("NEED_HARVESTER") != std::string::npos) {
        currentRole = Role::HARVESTER;
        roleAssigned = true;
    } else if (content.find("NEED_DISRUPTER") != std::string::npos) {
        currentRole = Role::DISRUPTER;
        roleAssigned = true;
    } else if (content.find("NEED_LEVELER") != std::string::npos) {
        currentRole = Role::LEVELER;
        roleAssigned = true;
    } else if (content.find("NEED_FEEDER") != std::string::npos) {
        currentRole = Role::FEEDER;
        roleAssigned = true;
    }
    if (roleAssigned) {
        currentPhase = AttributionPhase::ROLE_ASSIGNED;
        std::cout << "Role assigned from direct broadcast: "
                  << roleToString(currentRole) << std::endl;
        AI::Interface::i().sendMessage("ROLE_ACKNOWLEDGED_" +
            roleToString(currentRole));
    }
}

/**
 * @brief Process messages during the ID collection phase
 * @param content The message content
 */
void RoleAttributionModule::processIdCollectionPhase(
    const std::string& content) {
    if (content.find("ID_ATTRIBUTION") != std::string::npos) {
        if (myId > 0) {
            AI::Interface::i().sendMessage("I_AM_" + std::to_string(myId));
        }
        return;
    }

    if (content.find("I_AM_") != std::string::npos) {
        int id = extractNumber(content);
        if (id > 0) {
            receivedIds.insert(id);
            std::cout << "Received ID: " << id << std::endl;
        }
        return;
    }

    int id = extractNumber(content);
    if (id > 0) {
        receivedIds.insert(id);
        std::cout << "Received number in message: " << id << std::endl;
    }
}

/**
 * @brief Extract number from a message
 * @param content The message content
 * @return int The extracted number, or -1 if no number found
 */
int RoleAttributionModule::extractNumber(const std::string& content) {
    // Use regex to find any number in the string
    std::regex numberRegex("\\d+");
    std::smatch match;

    if (std::regex_search(content, match, numberRegex)) {
        try {
            return std::stoi(match[0]);
        } catch (const std::exception& e) {
            return -1;
        }
    }
    return -1;
}

/**
 * @brief Convert a role to a string representation
 * @param role The role to convert
 * @return std::string The string representation of the role
 */
std::string RoleAttributionModule::roleToString(Role role) const {
    switch (role) {
        case Role::UNKNOWN:
            return "UNKNOWN";
        case Role::HARVESTER:
            return "HARVESTER";
        case Role::DISRUPTER:
            return "DISRUPTER";
        case Role::LEVELER:
            return "LEVELER";
        case Role::FEEDER:
            return "FEEDER";
        default:
            return "INVALID_ROLE";
    }
}

/**
 * @brief Check if a role has been assigned
 * @return bool True if role is assigned
 */
bool RoleAttributionModule::isRoleAssigned() const {
    return currentPhase == AttributionPhase::ROLE_ASSIGNED;
}
