
#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <random>
#include <string>

// Utility functions and constants for the AI modules

namespace Utils {
    // Function to generate a random number between min and max (inclusive)
    int randomInt(int min, int max) {
        static std::random_device rd;  // Obtain a random number from hardware
        static std::mt19937 eng(rd());  // Seed the generator
        std::uniform_int_distribution<> distr(min, max); // Define the range
        return distr(eng);
    }

    // Function to log messages to the console
    void log(const std::string &message) {
        std::cout << "[LOG] " << message << std::endl;
    }
}

#endif // UTILS_HPP