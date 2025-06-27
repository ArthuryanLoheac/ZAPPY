/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Role Attribution Module Implementation
*/

#include <unistd.h>
#include <regex>
#include <random>
#include <chrono>
#include <string>
#include "modules/RoleAttributionModule.hpp"
#include "../Interface/Interface.hpp"
#include "../Data/Data.hpp"
#include "../../libc/include/logs.h"

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
    LOG_INFO("Role Attribution Module initialized with role: %s", roleToString(currentRole).c_str());
    LOG_INFO("Starting initial LOOK phase with PID-based random seed: %d", getpid());
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
                LOG_INFO("Role Attribution Module sending LOOK command %d/3...", lookCounter + 1);
                AI::Interface::i().sendCommand(LOOK);
                lookCounter++;
                if (lookCounter >= 3 && currentRole == Role::UNKNOWN) {
                    LOG_INFO("No role assigned from NEED messages. Moving to ID attribution phase...");
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
                LOG_INFO("Collecting IDs - LOOK command %d/3... %ld", lookCounter + 1, time(nullptr));
                AI::Interface::i().sendCommand(LOOK);
                lookCounter++;
                if (lookCounter >= 3) {
                    myId = generateRandomId();
                    LOG_INFO("Assigning random ID: %d", myId);
                    assignRoleFromId();
                    AI::Interface::i().sendMessage("I_AM_" + std::to_string(myId));
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
    LOG_INFO("Role assigned based on random ID: %s", roleToString(currentRole).c_str());
}

/**
 * @brief Reassign role using new random ID
 */
void RoleAttributionModule::reassignRole() {
    myId = generateRandomId();
    LOG_INFO("Reassigning with new random ID: %d", myId);
    assignRoleFromId();
    AI::Interface::i().sendMessage("NEW_ID_" + std::to_string(myId));
    LOG_INFO("Reassigned to: %s", roleToString(currentRole).c_str());
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

        LOG_INFO("Broadcast received - Content: '%s', Direction: %d", content.c_str(), direction);

        if (content.find("REROLL") != std::string::npos &&
            currentPhase == AttributionPhase::ROLE_ASSIGNED) {
            LOG_INFO("REROLL, reassigning role...");
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
        LOG_INFO("Role assigned from direct broadcast: %s", roleToString(currentRole).c_str());
        AI::Interface::i().sendMessage("ROLE_ACKNOWLEDGED_" + roleToString(currentRole));
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
            LOG_INFO("Received ID: %d", id);
        }
        return;
    }

    int id = extractNumber(content);
    if (id > 0) {
        receivedIds.insert(id);
        LOG_INFO("Received number in message: %d", id);
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
