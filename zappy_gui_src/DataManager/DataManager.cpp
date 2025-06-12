#include <string>

#include "DataManager/DataManager.hpp"

namespace GUI {
DataManager::DataManager() {
    std::lock_guard<std::mutex> lock(mutexDatas);
    port = 0;
    ip = "";
}

int DataManager::getPort() const {
    return port;
}

int DataManager::getFrequency() const {
    return frequency;
}

std::string DataManager::getIp() const {
    return ip;
}

void DataManager::setPort(int _port) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    if (_port < 0 || _port > 65535)
        throw ParseException("Port must be between 0 and 65535");
    port = _port;
}

void DataManager::setIp(std::string _ip) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    if (_ip.empty())
        throw ParseException("IP address cannot be empty");
    ip = _ip;
}

void DataManager::setRunning(bool b) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    running = b;
}

void DataManager::setFrequency(int f) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    frequency = f;
}

}  // namespace GUI
