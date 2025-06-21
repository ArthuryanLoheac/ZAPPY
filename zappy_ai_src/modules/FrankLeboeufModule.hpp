#pragma once

#include "modules/AIModule.hpp"

/**
 * @class FrankLeboeufModule
 * @brief AI module that broadcasts a promotional message as Frank Leboeuf.
 *
 * This module sends a predefined broadcast message when executed, simulating
 * a commercial for selling cars. It inherits from AIModule and can be used
 * as part of the AI decision system.
 */
class FrankLeboeufModule : public AIModule {
 public:
    /**
     * @brief Construct a new FrankLeboeufModule object.
     */
    FrankLeboeufModule() = default;

    /**
     * @brief Destroy the FrankLeboeufModule object.
     */
    ~FrankLeboeufModule() override = default;

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
};
