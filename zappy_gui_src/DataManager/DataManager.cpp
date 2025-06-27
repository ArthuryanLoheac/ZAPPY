#include <string>

#include "DataManager/DataManager.hpp"
#include "DataManager.hpp"

namespace GUI {

/**
 * @brief Constructs a new DataManager object and initializes default values.
 */
DataManager::DataManager() {
    std::lock_guard<std::mutex> lock(mutexDatas);
    port = 0;
    ip = "";
}

/**
 * @brief Gets the server port.
 *
 * @return int The server port.
 */
int DataManager::getPort() const {
    return port;
}

/**
 * @brief Gets the game frequency.
 *
 * @return int The game frequency.
 */
int DataManager::getFrequency() const {
    return frequency;
}

/**
 * @brief Gets the server IP address.
 *
 * @return std::string The server IP address.
 */
std::string DataManager::getIp() const {
    return ip;
}

bool DataManager::isOptimized() const {
    return optimized;
}

/**
 * @brief Sets the server port.
 *
 * @param _port The port to set (must be between 0 and 65535).
 * @throws ParseException If the port is out of range.
 */
void DataManager::setPort(int _port) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    if (_port < 0 || _port > 65535)
        throw ParseException("Port must be between 0 and 65535");
    port = _port;
}

/**
 * @brief Sets the server IP address.
 *
 * @param _ip The IP address to set.
 * @throws ParseException If the IP address is empty.
 */
void DataManager::setIp(std::string _ip) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    if (_ip.empty())
        throw ParseException("IP address cannot be empty");
    ip = _ip;
}

/**
 * @brief Sets the running state of the server.
 *
 * @param b The running state to set.
 */
void DataManager::setRunning(bool b) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    running = b;
}

/**
 * @brief Sets the game frequency.
 *
 * @param f The frequency to set.
 */
void DataManager::setFrequency(int f) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    frequency = f;
}

/**
 * @brief Sets the optimization state.
 *
 * @param o The optimization state to set.
 */
void DataManager::setOptimized(bool o) {
    std::lock_guard<std::mutex> lock(mutexDatas);
    optimized = o;
}
/**
 * @brief Clears the DataManager state.
 *
 * This method resets frequency, and running state to their default values.
 */
void DataManager::clear()  {
    std::lock_guard<std::mutex> lock(mutexDatas);
    frequency = 0;
    running = true;
}
}  // namespace GUI
