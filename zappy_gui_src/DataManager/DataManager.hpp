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
 private:
    bool debug;

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
    int getPort() const;
    std::string getIp() const;

    void setDebug(bool isDebug);
    void setPort(int port);
    void setIp(std::string ip);
    void setRunning(bool b);
};
}  // namespace GUI
