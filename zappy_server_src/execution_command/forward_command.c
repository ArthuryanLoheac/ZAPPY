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
    if (client->stats.orientation == 3)
        client->stats.y = (client->stats.y + 1) % zappy->parser->height;
    if (client->stats.orientation == 2)
        client->stats.x = (client->stats.x + 1) % zappy->parser->width;
    if (client->stats.orientation == 1) {
        client->stats.y -= 1;
        if (client->stats.y < 0)
            client->stats.y = zappy->parser->height - 1;
    }
    if (client->stats.orientation == 4) {
        client->stats.x -= 1;
        if (client->stats.x < 0)
            client->stats.x = zappy->parser->width - 1;
    }
    update_pos_player(zappy, client);
    add_to_buffer(&client->out_buffer, "ok\n");
}
