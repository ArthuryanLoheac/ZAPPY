/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** forward_command
*/

#include <stdio.h>
#include "command_handler.h"

void forward_command(zappy_t *zappy, client_t *client, char **args)
{
    (void) args;
    if (client->orientation == 3)
        client->y = (client->y + 1) % zappy->parser->height;
    if (client->orientation == 2)
        client->x = (client->x + 1) % zappy->parser->width;
    if (client->orientation == 1) {
        client->y -= 1;
        if (client->y < 0)
            client->y = zappy->parser->height - 1;
    }
    if (client->orientation == 4) {
        client->x -= 1;
        if (client->x < 0)
            client->x = zappy->parser->width - 1;
    }
    update_pos_player(zappy, client);
    add_to_buffer(&client->out_buffer, "ok\n");
}

