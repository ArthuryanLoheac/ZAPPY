/*
** EPITECH PROJECT, 2025
** Plazza
** File description:
** Fork
*/

#pragma once

#include <unistd.h>
#include <functional>
#include <chrono>
#include <tuple>
#include <utility>
#include <type_traits>
#include <algorithm>

/**
 * @class Fork
 * @brief A wrapper class for Unix fork() system call
 *
 * This class provides a C++ wrapper around the fork() system call,
 * with additional utility methods for process management.
 * It allows easy creation of child processes and provides methods for
 * communication, waiting, and signal handling between parent and child.
 */
class Fork {
 public:
    /**
     * @brief Default constructor
     * 
     * Creates a new process by calling fork().
     * After construction, you can check isChild() or isParent() to determine
     * which process is executing.
     * 
     * @throw std::runtime_error if fork() fails
     */
    Fork();

    /**
     * @brief Constructor that forks and executes the provided function in the child process
     * 
     * @tparam Func Type of the function to execute
     * @tparam Args Types of the arguments to pass to the function
     * @param func Function to execute in the child process
     * @param args Arguments to pass to the function
     * @throw std::runtime_error if fork() fails
     */
    template<typename Func, typename... Args>
    explicit Fork(Func&& func, Args&&... args) {
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
            func(std::forward<Args>(args)...);
            exit(0);
        } else {
            _isParent = true;
            _isForked = true;
        }
    }

    /**
     * @brief Destructor
     * 
     * If this is the parent process, it will wait for the child to terminate.
     */
    ~Fork();

    // Delete copy and move operations
    Fork(const Fork &) = delete;
    Fork &operator=(const Fork &) = delete;
    Fork(Fork &&) = delete;
    Fork &operator=(Fork &&) = delete;

    /**
     * @brief Wait for the child process to terminate (blocking)
     * 
     * Blocks until the child process terminates.
     */
    void wait();

    /**
     * @brief Terminate the child process by sending SIGKILL
     * 
     * Sends SIGKILL signal to the child process to terminate it immediately.
     */
    void terminateChild();

    /**
     * @brief Non-blocking wait for child process
     * 
     * Checks if the child process has terminated without blocking.
     * 
     * @return true if the child has terminated, false otherwise
     */
    bool waitNoHang();

    /**
     * @brief Wait for child with timeout
     * 
     * Waits for the child process to terminate for a specified number of seconds.
     * 
     * @param seconds Maximum time to wait in seconds
     * @return true if the child terminated within the timeout, false otherwise
     */
    bool waitTimeout(unsigned int seconds);

    /**
     * @brief Send a signal to the child process
     * 
     * @param signal Signal number to send
     * @return true if the signal was sent successfully, false otherwise
     */
    bool sendSignal(int signal) const;

    /**
     * @brief Check if the child process has exited
     * 
     * @return true if the child has exited, false otherwise
     */
    bool hasExited() const;

    /**
     * @brief Get the exit status of the child process
     * 
     * @return The exit status if the child exited normally, -1 otherwise
     */
    int getExitStatus() const;

    /**
     * @brief Check if the child process was terminated by a signal
     * 
     * @return true if the child was terminated by a signal, false otherwise
     */
    bool wasSignaled() const;

    /**
     * @brief Get the signal that terminated the child process
     * 
     * @return The terminating signal number if the child was terminated by a signal, -1 otherwise
     */
    int getTerminatingSignal() const;

    /**
     * @brief Check if this is the child process
     * 
     * @return true if this is the child process, false otherwise
     */
    bool isChild() const;

    /**
     * @brief Check if this is the parent process
     * 
     * @return true if this is the parent process, false otherwise
     */
    bool isParent() const;

    /**
     * @brief Get the process ID of the child
     * 
     * @return The PID of the child process
     */
    pid_t getPid() const;

    /**
     * @brief Check if the fork was successful
     * 
     * @return true if the fork was successful, false otherwise
     */
    bool isForked() const;

 private:
    pid_t _pid;      ///< Process ID of the child
    int _status;     ///< Status of the child process
    bool _isChild;   ///< Flag indicating if this is the child process
    bool _isParent;  ///< Flag indicating if this is the parent process
    bool _isForked;  ///< Flag indicating if fork was successful
    bool _hasExited;  ///< Flag indicating if the child has exited
};
