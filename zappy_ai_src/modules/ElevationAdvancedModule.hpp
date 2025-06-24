#pragma once

#include "modules/AIModule.hpp"

/**
 * @class ElevationAdvancedModule
 * @brief AI module that broadcasts a promotional message as Frank Leboeuf.
 *
 * This module sends a predefined broadcast message when executed, simulating
 * a commercial for selling cars. It inherits from AIModule and can be used
 * as part of the AI decision system.
 */
class ElevationAdvancedModule : public AIModule {
 public:
    /**
     * @brief Construct a new ElevationAdvancedModule object.
     */
    ElevationAdvancedModule() = default;

    /**
     * @brief Destroy the ElevationAdvancedModule object.
     */
    ~ElevationAdvancedModule() override = default;

    /**
     * @brief Execute the module's main action.
     *
     * Sends a broadcast message as Frank Leboeuf.
     */
    void execute() override;

    /**
     * @brief Get the priority of this module.
     *
     * @return The priority value as a float.
     */
    float getPriority() override;

 private:
   int inventoryCheckCount = 0;
   int foodCount = 0;
   int level = 0;
};
