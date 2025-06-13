/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/

#include "CommunicationModule.hpp"
#include <iostream>

CommunicationModule::CommunicationModule() {
    // Initialization code if needed
}

std::string CommunicationModule::execute() {
    // Logic for broadcasting messages to other players
    std::cout << "Broadcasting message to other players..." << std::endl;
    // Example: return a broadcast command
    return "Broadcast Hello";
}

float CommunicationModule::getPriority() const {
    // Define the priority for this module (between 0 and 1)
    return 0.5f; // Example priority value
}