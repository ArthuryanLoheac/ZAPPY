/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** player n°i's position
*/

#include <stdio.h>

#include "command_handler.h"
#include "pointlen.h"
#include "logs.h"

void ppo_command(zappy_t *zappy, client_t *client, char **args)
{
    char response[20];

    (void) zappy;
    if (pointlen(args) != 2) {
        LOG_WARNING("[%i]: Wrong amount of arguments for command ppo."
            " Got %i but required %i", client->fd, pointlen(args), 2);
    } else {
        snprintf(response, 19, "ppo %i %i %i %i\n", client->stats.id,
            client->stats.x, client->stats.y, client->stats.orientation);
    }
}
