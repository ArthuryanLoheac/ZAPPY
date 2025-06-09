#include <string>

#include "DataManager/DataManager.hpp"

namespace AI {
DataManager::DataManager() {
    debug = NO_DEBUG;
    port = 0;
    ip = "";
}

bool DataManager::getDebug() const {
    return debug == ALL_DEBUG;
}

bool DataManager::getErrors() const {
    return (debug == ALL_DEBUG) || (debug == ERRORS);
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

std::string DataManager::getTeam() const {
    return team;
}

void DataManager::setDebug(debugMode isDebug) {
    debug = isDebug;
}

void DataManager::setPort(int _port) {
    if (_port < 0 || _port > 65535)
        throw ParseException("Port must be between 0 and 65535");
    port = _port;
}

void DataManager::setIp(std::string _ip) {
    if (_ip.empty())
        throw ParseException("IP address cannot be empty");
    ip = _ip;
}

void DataManager::setRunning(bool b) {
    running = b;
}

void DataManager::setFrequency(int f) {
    frequency = f;
}

void DataManager::setTeam(std::string _team) {
    if (_team.empty())
        throw ParseException("Team name cannot be empty");
    team = _team;
}

}  // namespace AI
