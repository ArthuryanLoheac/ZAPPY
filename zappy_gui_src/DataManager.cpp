#include "DataManager.hpp"

namespace GUI
{
DataManager::DataManager()
{
    std::lock_guard<std::mutex> lock(mutexDatas);
    debug = false;
    port = 0;
    ip = "";
}

bool DataManager::getDebug() const {
    return debug;
}

int DataManager::getPort() const {
    return port;
}

std::string DataManager::getIp() const {
    return ip;
}

void DataManager::setDebug(bool isDebug) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    debug = isDebug;
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

} // namespace GUI