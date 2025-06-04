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

class Fork {
 public:
    // Default constructor
    Fork();

    // Constructor for a function with arguments
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

    // Destructor and block copy/move operations
    ~Fork();
    Fork(const Fork &) = delete;
    Fork &operator=(const Fork &) = delete;
    Fork(Fork &&) = delete;
    Fork &operator=(Fork &&) = delete;

    // Methods
    void wait();
    void terminateChild();
    bool waitNoHang();
    bool waitTimeout(unsigned int seconds);
    bool sendSignal(int signal);
    bool hasExited() const;
    int getExitStatus() const;
    bool wasSignaled() const;
    int getTerminatingSignal() const;

    // Getters / Setters
    bool isChild() const;
    bool isParent() const;
    pid_t getPid() const;
    bool isForked() const;

 private:
    pid_t _pid;
    int _status;
    bool _isChild;
    bool _isParent;
    bool _isForked;
    bool _hasExited;
};
