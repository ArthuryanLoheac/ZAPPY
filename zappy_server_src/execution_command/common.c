/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Utility functions used by multiple comands handlers
*/

#include <stdio.h>

#include "command_handler.h"

void update_pos_player(zappy_t *zappy, client_t *client)
{
    char buffer[256];

    sprintf(buffer, "ppo #%d %d %d %d\n", client->id, client->x, client->y,
        client->orientation);
    send_data_to_graphics(zappy, buffer);
}
