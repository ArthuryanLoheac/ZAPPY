#pragma once
#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <map>

#include "Exceptions/DataManagerExceptions.hpp"

namespace AI {
class DataManager {
 public:
    enum debugMode {
        NO_DEBUG,
        ALL_DEBUG,
        ERRORS
    };

 private:
    debugMode debug = NO_DEBUG;

    int port;
    std::string ip;
    std::string team;
    int frequency = 0;

 public:
    bool running = true;
    static DataManager &i() {
        static DataManager i;
        return i;
    }

    DataManager();
    bool getDebug() const;
    bool getErrors() const;
    int getPort() const;
    int getFrequency() const;
    std::string getIp() const;
    std::string getTeam() const;

    void setDebug(debugMode isDebug);
    void setPort(int port);
    void setIp(std::string ip);
    void setTeam(std::string ip);
    void setRunning(bool b);
    void setFrequency(int f);
};
}  // namespace AI
