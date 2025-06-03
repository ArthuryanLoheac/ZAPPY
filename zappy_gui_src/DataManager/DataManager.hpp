#pragma once
#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <map>
#include <mutex>

#include "Exceptions/DataManagerExceptions.hpp"

namespace GUI {
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
 public:
    bool running = true;
    std::mutex mutexDatas;
    static DataManager &i() {
        static DataManager i;
        return i;
    }

    DataManager();
    bool getDebug() const;
    bool getErrors() const;
    int getPort() const;
    std::string getIp() const;

    void setDebug(debugMode isDebug);
    void setPort(int port);
    void setIp(std::string ip);
    void setRunning(bool b);
};
}  // namespace GUI
