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
    int port;
    std::string ip;
    int frequency = 0;
    bool optimized = false;

 public:
    bool running = true;
    std::mutex mutexDatas;
    static DataManager &i() {
        static DataManager i;
        return i;
    }

    DataManager();
    int getPort() const;
    int getFrequency() const;
    std::string getIp() const;
    bool isOptimized() const;

    void setPort(int port);
    void setIp(std::string ip);
    void setRunning(bool b);
    void setFrequency(int f);
    void setOptimized(bool o);

    void clear();
};
}  // namespace GUI
