/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** main
*/

#include <iostream>
#include "ai/PrioritySystem.hpp"
#include "modules/FoodGatheringModule.hpp"
#include "modules/CommunicationModule.hpp"

int main() {
    PrioritySystem prioritySystem;

    FoodGatheringModule foodModule;
    CommunicationModule commModule;

    prioritySystem.addModule(&foodModule);
    prioritySystem.addModule(&commModule);

    while (true) {
        std::string serverResponse;
        prioritySystem.executeHighestPriorityModule();
        std::cout << "Enter server response (or type 'exit' to quit): ";
        if (std::getline(std::cin, serverResponse)) {
            prioritySystem.handleServerResponse(serverResponse);
        }

    }

    return 0;
}