/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** player n°i's inventory
*/

#include <stdio.h>
#include <stdlib.h>

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
}

void pin_command(zappy_t *zappy, client_t *client, char **args)
{
    int id;

    (void) client;
    if (pointlen(args) != 1) {
        LOG_WARNING("[%i]: Wrong amount of arguments for command pin."
            " Got %i but required %i", client->fd, pointlen(args), 1);
    } else {
        id = atoi(&args[1][1]);
        send_player_inventory(zappy, client, id);
    }
}
