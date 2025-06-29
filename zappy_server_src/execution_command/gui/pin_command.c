/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** player n°i's inventory
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_handler.h"
#include "pointlen.h"
#include "logs.h"

void send_player_inventory(zappy_t *zappy, client_t *gui, int id)
{
    char response[100];

    for (client_t *actual = zappy->clients; actual != NULL;
        actual = actual->next) {
        if (actual->stats.id == id) {
            snprintf(response, 99, "pin #%i %d %d %i %i %i %i %i %i %i\n",
                actual->stats.id, actual->stats.x, actual->stats.y,
                actual->stats.inventory.food, actual->stats.inventory.linemate,
                actual->stats.inventory.deraumere,
                actual->stats.inventory.sibur,
                actual->stats.inventory.mendiane,
                actual->stats.inventory.phiras,
                actual->stats.inventory.thystame
            );
            add_to_buffer(&gui->out_buffer, response);
            return;
        }
    }
    LOG_WARNING("[%i]: Unknown player id(%i) from pin gui request",
        gui->fd, id);
    add_to_buffer(&gui->out_buffer, "ko\n");
}

void pin_command(zappy_t *zappy, client_t *client, char **args)
{
    int id;

    if (!client) {
        LOG_WARNING("Invalid arguments for command pin.");
        return;
    }
    if (!args || !args[0] || strlen(args[0]) < 2) {
        LOG_WARNING("Invalid arguments for command pin.");
        add_to_buffer(&client->out_buffer, "ko\n");
        return;
    }
    if (args[0][1] < '0' || args[0][1] > '9') {
        LOG_WARNING("[%i]: Invalid player id for pin command: %s",
            client->fd, args[0]);
        add_to_buffer(&client->out_buffer, "ko\n");
        return;
    }
    id = atoi(args[0] + 1);
    send_player_inventory(zappy, client, id);
}
