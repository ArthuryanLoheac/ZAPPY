/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** player n°i's level
*/

#include <stdio.h>
#include <stdlib.h>

#include "command_handler.h"
#include "pointlen.h"
#include "logs.h"

void send_player_level(zappy_t *zappy, client_t *gui, int id)
{
    char response[20];

    for (client_t *actual = zappy->clients; actual != NULL;
        actual = actual->next) {
        if (actual->stats.id == id) {
            snprintf(response, 19, "plv #%i %i\n",
                actual->stats.id, actual->stats.level);
            add_to_buffer(&gui->out_buffer, response);
            return;
        }
    }
    LOG_WARNING("[%i]: Unknown player id(%i) from plv gui request",
        gui->fd, id);
}

void plv_command(zappy_t *zappy, client_t *client, char **args)
{
    int id;

    (void) client;
    if (pointlen(args) != 1) {
        LOG_WARNING("[%i]: Wrong amount of arguments for command plv."
            " Got %i but required %i", client->fd, pointlen(args), 1);
    } else {
        id = atoi(&args[1][1]);
        send_player_level(zappy, client, id);
    }
}
