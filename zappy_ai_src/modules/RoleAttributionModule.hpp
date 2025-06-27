/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Role Attribution Module
*/

#ifndef ZAPPY_AI_SRC_MODULES_ROLEATTRIBUTIONMODULE_HPP_
#define ZAPPY_AI_SRC_MODULES_ROLEATTRIBUTIONMODULE_HPP_

#include <string>
#include <set>
#include <random>
#include "modules/AIModule.hpp"

/**
 * @brief Enum defining possible AI roles in the game
 */
enum class Role {
    UNKNOWN,    // Initial state, role not determined yet
    HARVESTER,  // Focused on gathering food resources
    DISRUPTER,  // Focused on exploring and disrupting enemy elevation
    LEVELER,    // Focused on collecting materials for elevation
    FEEDER,     // Focused on providing food to other players
};

/**
 * @brief Enum defining the phases of role attribution
 */
enum class AttributionPhase {
    INITIAL_LOOK,       // Initial phase sending 3 LOOK commands
    BROADCASTING_ID,    // Broadcasting ID_ATTRIBUTION
    COLLECTING_IDS,     // Collecting IDs from other players
    ROLE_ASSIGNED       // Role has been assigned
};

/**
 * @brief Module responsible for determining the AI's role
 *
 * This module processes broadcast messages to determine what role
 * the AI should take based on team needs and communications.
 */
class RoleAttributionModule : public AIModule {
 public:
    /**
     * @brief Constructor for the RoleAttributionModule
     */
    RoleAttributionModule();

    /**
     * @brief Execute the role attribution behavior
     *
     * Periodically sends LOOK commands and processes messages
     */
    void execute() override;

    /**
     * @brief Calculate the priority of this module
     * @return float Priority value between 0.0 and 1.0
     */
    float getPriority() override;

    /**
     * @brief Get the current role of the AI
     * @return Role The current role
     */
    Role getCurrentRole() const;

    /**
     * @brief Check if a role has been assigned
     * @return bool True if role is assigned
     */
    bool isRoleAssigned() const;

    /**
     * @brief Reassign role using new random ID
     */
    void reassignRole();

 private:
    /**
     * @brief Process incoming broadcast messages
     */
    void processMessages();

    /**
     * @brief Process messages during the initial phase
     * @param content The message content
     * @param direction The message direction
     */
    void processInitialPhase(const std::string& content, int direction);

    /**
     * @brief Process messages during the ID collection phase
     * @param content The message content
     */
    void processIdCollectionPhase(const std::string& content);

    /**
     * @brief Extract number from a message
     * @param content The message content
     * @return int The extracted number, or -1 if no number found
     */
    int extractNumber(const std::string& content);

    /**
     * @brief Generate a random ID
     * @return int A random integer ID
     */
    int generateRandomId();

    /**
     * @brief Assign role based on ID
     */
    void assignRoleFromId();

    /**
     * @brief Convert a role to a string representation
     * @param role The role to convert
     * @return std::string The string representation of the role
     */
    std::string roleToString(Role role) const;

    Role currentRole;

    AttributionPhase currentPhase;

    int lookCounter;

    std::set<int> receivedIds;

    int myId;

    std::mt19937 rng;

    std::uniform_int_distribution<int> idDistribution;
};

#endif  // ZAPPY_AI_SRC_MODULES_ROLEATTRIBUTIONMODULE_HPP_
