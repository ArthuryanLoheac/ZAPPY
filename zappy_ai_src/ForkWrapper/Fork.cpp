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

Fork::~Fork() {
    if (_isForked) {
        waitpid(_pid, &_status, 0);
    }
}

void Fork::wait() {
    if (_isForked) {
        waitpid(_pid, &_status, 0);
        _isForked = false;
        _hasExited = true;
    }
}

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

bool Fork::sendSignal(int signal) const {
    if (_isForked) {
        return (kill(_pid, signal) == 0);
    }
    return false;
}

bool Fork::hasExited() const {
    return _hasExited;
}

int Fork::getExitStatus() const {
    if (_hasExited && WIFEXITED(_status))
        return WEXITSTATUS(_status);
    return -1;
}

bool Fork::wasSignaled() const {
    return _hasExited && WIFSIGNALED(_status);
}

int Fork::getTerminatingSignal() const {
    if (_hasExited && WIFSIGNALED(_status))
        return WTERMSIG(_status);
    return -1;
}

bool Fork::isChild() const {
    return _isChild;
}

bool Fork::isParent() const {
    return _isParent;
}

pid_t Fork::getPid() const {
    return _pid;
}

bool Fork::isForked() const {
    return _isForked;
}

void Fork::terminateChild() {
    if (_isForked) {
        kill(_pid, SIGKILL);
        _isForked = false;
    }
}
