#pragma once
#include <poll.h>

#include <string>
#include <queue>
#include <vector>
#include <map>

namespace AI {
class ServerAI {
 public:
    int server_fd;
    int nb_fds;
    struct pollfd fd;
    std::string buffer;
    bool waitingPos = false;
    bool waitingId = false;

    ServerAI();
    static ServerAI &i() {
        static ServerAI i;
        return i;
    }
    
    // Main server loop
    void startServer();
    
    // Message handling methods
    std::string getNextMessage();
    void queueMessage(const std::string &message);
    void sendQueuedMessages();
    void sendDatasToServer(const std::string &message);
    
    // Command handling
    void handleCommand();
    void processServerResponse(const std::string& response);
    
    // Response parsing
    std::vector<std::string> parseCommands(std::string &command);
    void parseInventoryResponse(const std::string& response);
    std::map<std::string, int> extractInventoryItems(const std::string& inventoryStr);

    std::string lastCommand;

 private:
    std::queue<std::string> sendQueue;  // Queue for outgoing messages
    
    // Network operations
    void readDatasFromServer();
    void handleIncomingData();
    void handleOutgoingData();
    
    // Command processing
    void processCommand(std::string& command);
    void executeServerCommand(const std::string& cmdName, const std::vector<std::string>& args);
    
    // Welcome handshake methods
    void welcomeCommand(std::vector<std::string> &args);
    void returnWelcomeCommand(std::vector<std::string> &args);
    void processWelcomePosition(const std::vector<std::string> &args);
    void processWelcomeId(const std::vector<std::string> &args);

    // Command map for server commands
    std::map<std::string, void(ServerAI::*)(std::vector<std::string> &)> commands = {
        {"WELCOME", &ServerAI::welcomeCommand}
    };
};
}  // namespace AI
