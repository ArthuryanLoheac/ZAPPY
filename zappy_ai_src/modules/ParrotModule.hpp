#pragma once

#include <vector>
#include <string>

#include "modules/AIModule.hpp"

class ParrotModule : public AIModule {
 public:
    ParrotModule() = default;
    ~ParrotModule() override = default;

    void execute() override;
    float getPriority() override;

    void fuckUpMessages(const std::vector<std::string>& messages);
    std::vector<std::string> chooseRandomMessages(int count);

 protected:
 private:
};
