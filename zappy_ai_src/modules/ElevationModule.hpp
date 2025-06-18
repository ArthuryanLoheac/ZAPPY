/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Elevation Module
*/

#ifndef ZAPPY_AI_SRC_MODULES_ELEVATIONMODULE_HPP_
#define ZAPPY_AI_SRC_MODULES_ELEVATIONMODULE_HPP_

#include <map>
#include "modules/AIModule.hpp"

/**
 * @brief Module responsible for managing the AI's elevation/leveling process
 *
 * This module handles finding linemate stones, navigating to them, and performing
 * incantations to level up the AI. It ensures sufficient food resources are available
 * before starting incantations.
 */
class ElevationModule : public AIModule {
 public:
    /**
     * @brief Constructor for the ElevationModule
     */
    ElevationModule();

    /**
     * @brief Execute the elevation behavior
     *
     * Manages the process of finding linemate, moving to it, and performing incantation
     */
    void execute() override;

    /**
     * @brief Calculate the priority of this module
     *
     * @return float Priority value between 0.0 and 1.0
     */
    float getPriority() override;

 private:
    /**
     * @brief Check resource availability for elevation
     *
     * Updates internal status flags based on current resources
     */
    void checkResources();

    /**
     * @brief Look for suitable elevation spots
     *
     * @return bool True if a suitable spot was found
     */
    bool findElevationSpot();

    /**
     * @brief Check the current tile for linemate
     *
     * @return bool True if linemate is on current tile
     */
    bool checkCurrentTileForLinemate();

    /**
     * @brief Scan the vision for tiles with linemate
     *
     * @return bool True if a tile with linemate was found
     */
    bool scanVisionForLinemate();

    /**
     * @brief Handle the elevation process
     *
     * Manages dropping linemate or moving to a linemate tile, then starting incantation
     */
    void performElevation();

    /**
     * @brief Handle the case when linemate is in inventory
     */
    void handleLinemateInInventory();

    /**
     * @brief Handle the case when linemate was found on a tile
     */
    void handleFoundLinemateTile();

    /// Location of a suitable elevation spot (tile with linemate)
    int targetX = -1;
    int targetY = -1;

    /// Status flags
    bool hasSufficientFood = false;
    bool hasLinemate = false;
    bool foundSpot = false;

    /// Current food count
    int foodCount = 0;
};

#endif  // ZAPPY_AI_SRC_MODULES_ELEVATIONMODULE_HPP_
