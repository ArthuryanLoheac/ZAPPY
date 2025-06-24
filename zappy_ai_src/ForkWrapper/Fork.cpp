/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Fork
*/

#include <sys/types.h>
#include <sys/wait.h>

#include <csignal>
#include <chrono>
#include <thread>
#include <stdexcept>

#include "ForkWrapper/Fork.hpp"

/**
 * @brief Constructor for the Fork class
 *
 * Initializes the object and immediately calls fork().
 * After construction, either isChild() or isParent() will be true.
 *
 * @throw std::runtime_error If the fork() system call fails
 */
Fork::Fork() {
    _pid = 0;
    _status = 0;
    _isChild = false;
    _isParent = false;
    _isForked = false;
    _hasExited = false;

    _pid = fork();

    if (_pid == -1) {
        throw std::runtime_error("Fork failed");
    } else if (_pid == 0) {
        _isChild = true;
        _isForked = true;
    } else {
        _isParent = true;
        _isForked = true;
    }
}

/**
 * @brief Destructor for the Fork class
 *
 * If this is the parent process, waits for the child to terminate.
 * This prevents zombie processes when the parent is destroyed.
 */
Fork::~Fork() {
    if (_isForked && _isParent) {
        waitpid(_pid, &_status, 0);
    }
}

/**
 * @brief Wait for the child process to terminate
 *
 * Blocks until the child process terminates.
 * Updates _isForked and _hasExited flags accordingly.
 */
void Fork::wait() {
    if (_isForked) {
        waitpid(_pid, &_status, 0);
        _isForked = false;
        _hasExited = true;
    }
}

/**
 * @brief Non-blocking check if the child process has terminated
 *
 * Checks if the child process has terminated without blocking the parent.
 *
 * @return true if the child has terminated, false otherwise
 */
bool Fork::waitNoHang() {
    if (_isForked) {
        pid_t result = waitpid(_pid, &_status, WNOHANG);
        if (result == _pid) {
            _isForked = false;
            _hasExited = true;
            return true;
        }
    }
    return false;
}

/**
 * @brief Wait for the child process to terminate with a timeout
 *
 * Waits for the child to terminate, but only for the specified number of seconds.
 *
 * @param seconds Maximum time to wait in seconds
 * @return true if the child terminated within the timeout, false otherwise
 */
bool Fork::waitTimeout(unsigned int seconds) {
    if (!_isForked)
        return true;

    auto start = std::chrono::steady_clock::now();
    auto timeout = std::chrono::seconds(seconds);

    while (std::chrono::steady_clock::now() - start < timeout) {
        if (waitNoHang())
            return true;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return false;
}

/**
 * @brief Send a signal to the child process
 *
 * @param signal Signal number to send
 * @return true if the signal was sent successfully, false otherwise
 */
bool Fork::sendSignal(int signal) const {
    if (_isForked && !_hasExited) {
        return (kill(_pid, signal) == 0);
    }
    return false;
}

/**
 * @brief Check if the child process has exited
 *
 * @return true if the child has exited, false otherwise
 */
bool Fork::hasExited() const {
    return _hasExited;
}

/**
 * @brief Get the exit status of the child process
 *
 * @return The exit status if the child exited normally, -1 otherwise
 */
int Fork::getExitStatus() const {
    if (_hasExited && WIFEXITED(_status))
        return WEXITSTATUS(_status);
    return -1;
}

/**
 * @brief Check if the child process was terminated by a signal
 *
 * @return true if the child was terminated by a signal, false otherwise
 */
bool Fork::wasSignaled() const {
    return _hasExited && WIFSIGNALED(_status);
}

/**
 * @brief Get the signal that terminated the child process
 *
 * @return The terminating signal number if the child was terminated by a signal, -1 otherwise
 */
int Fork::getTerminatingSignal() const {
    if (_hasExited && WIFSIGNALED(_status))
        return WTERMSIG(_status);
    return -1;
}

/**
 * @brief Check if this is the child process
 *
 * @return true if this is the child process, false otherwise
 */
bool Fork::isChild() const {
    return _isChild;
}

/**
 * @brief Check if this is the parent process
 *
 * @return true if this is the parent process, false otherwise
 */
bool Fork::isParent() const {
    return _isParent;
}

/**
 * @brief Get the process ID of the child
 *
 * @return The PID of the child process
 */
pid_t Fork::getPid() const {
    return _pid;
}

/**
 * @brief Check if the fork was successful
 *
 * @return true if the fork was successful, false otherwise
 */
bool Fork::isForked() const {
    return _isForked;
}

/**
 * @brief Terminate the child process by sending SIGKILL
 *
 * Sends SIGKILL signal to the child process and updates state variables.
 */
void Fork::terminateChild() {
    if (_isForked) {
        kill(_pid, SIGKILL);
        _isForked = false;
        _hasExited = true;
    }
}
