#pragma once
#include <sys/poll.h>
#include <stdexcept>

class PollWrapper {
 public:
    struct pollfd fd;

    PollWrapper() = default;

    void waitForEvent() {
        int ready = poll(&fd, 1, -1);
        if (ready == -1)
            throw std::runtime_error("Poll error occurred");
    }

    bool isReadable() const {
        return fd.revents & POLLIN;
    }

    bool isWritable() const {
        return fd.revents & POLLOUT;
    }
};
