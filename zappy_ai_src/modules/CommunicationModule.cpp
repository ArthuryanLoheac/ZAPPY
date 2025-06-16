/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/

#include "CommunicationModule.hpp"
#include <iostream>
#include "../Interface/Interface.hpp"
#include "../Data/Data.hpp"

CommunicationModule::CommunicationModule() {
    // Initialization code if needed
}

void CommunicationModule::execute() {
    static int inventoryCheckCount = 0;
    inventoryCheckCount++;
    // Logic for broadcasting messages to other players
    std::cout << "Broadcasting message to other players..." << std::endl;
    std::cout << "Current level: " << AI::Data::i().level << std::endl;
    
    if (inventoryCheckCount % 10 == 0) {
        std::cout << "Checking inventory..." << std::endl;
        AI::Interface::i().sendCommand(INVENTORY);
    }
    AI::Interface::i().sendCommand(RIGHT);
}

float CommunicationModule::getPriority() {
    // Define the priority for this module (between 0 and 1)
    return 0.5f; // Example priority value
}