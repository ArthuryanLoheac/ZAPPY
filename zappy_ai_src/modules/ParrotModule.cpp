#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <random>

#include "modules/ParrotModule.hpp"
#include "Interface/Interface.hpp"
#include "Data/Data.hpp"

void ParrotModule::execute() {
    if (AI::Data::i().messageQueue.empty()) {
        return;
    }
    std::vector<std::string> messages = chooseRandomMessages(5);

    fuckUpMessages(messages);
}

std::vector<std::string> ParrotModule::chooseRandomMessages(int count) {
    std::vector<std::string> messages;
    std::vector<std::string> allMessages;

    while (!AI::Data::i().messageQueue.empty()) {
        allMessages.push_back(AI::Data::i().messageQueue.front().first);
        AI::Data::i().messageQueue.pop();
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(allMessages.begin(), allMessages.end(), g);
    count = std::min<size_t>(count, allMessages.size());
    messages.assign(allMessages.begin(), allMessages.begin() + count);
    while (messages.size() < count && !allMessages.empty()) {
        messages.push_back(allMessages[messages.size() % allMessages.size()]);
    }
    return messages;
}

void ParrotModule::fuckUpMessages(const std::vector<std::string>& messages) {
    for (const auto& message : messages) {
        std::string modified = message;
        for (size_t i = std::max<int>(0, modified.size() - 5); i
            < modified.size(); ++i) {
            if (std::isdigit(modified[i])) {
                modified[i] = ((modified[i] - '0' + 1) % 10) + '0';
            }
        }
        AI::Interface::i().sendCommand("Broadcast " + modified + "\n");
    }
}

float ParrotModule::getPriority() {
    return 1.f;
}
