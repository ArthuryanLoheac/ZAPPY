/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/

#ifndef AIBASE_HPP
#define AIBASE_HPP

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>

class AIBase {
public:
    using MessageCallback = std::function<void(const std::string&)>;

    virtual ~AIBase() = default;

    // Subscribe a module to a specific message type (e.g., "Inventory", "Look", etc.)
    void subscribe(const std::string& messageType, MessageCallback callback) {
        subscribers[messageType].push_back(callback);
    }

    // Called when a message is received from the server
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

#endif // AIBASE_HPP