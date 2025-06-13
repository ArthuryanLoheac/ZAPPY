/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** take_command
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "command_handler.h"

void broadcast_command(zappy_t *zappy, client_t *client, char **args)
{
    char buffer[256];

    sprintf(buffer, "pbc #%d %s\n", client->stats.id, args[0]);
    send_data_to_graphics(zappy, buffer);
}
