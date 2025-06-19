/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/

#ifndef ZAPPY_AI_SRC_LOGIC_AIBASE_HPP_
#define ZAPPY_AI_SRC_LOGIC_AIBASE_HPP_

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>

class AIBase {
 public:
    using MessageCallback = std::function<void(const std::string&)>;

    virtual ~AIBase() = default;

    void subscribe(const std::string& messageType, MessageCallback callback) {
        subscribers[messageType].push_back(callback);
    }

    void notify(const std::string& messageType, const std::string& message) {
        auto it = subscribers.find(messageType);
        if (it != subscribers.end()) {
            for (auto& cb : it->second) {
                cb(message);
            }
        }
    }

 protected:
    std::unordered_map<std::string, std::vector<MessageCallback>> subscribers;
};

#endif  // ZAPPY_AI_SRC_LOGIC_AIBASE_HPP_
