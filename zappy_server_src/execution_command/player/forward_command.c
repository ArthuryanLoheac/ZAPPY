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
    stats_t *stats = &client->stats;

    (void) args;
    if (stats->orientation == 3)
        stats->y = (stats->y + 1) % zappy->parser->height;
    if (stats->orientation == 2)
        stats->x = (stats->x + 1) % zappy->parser->width;
    if (stats->orientation == 1) {
        stats->y -= 1;
        if (stats->y < 0)
            stats->y = zappy->parser->height - 1;
    }
    if (stats->orientation == 4) {
        stats->x -= 1;
        if (stats->x < 0)
            stats->x = zappy->parser->width - 1;
    }
    update_pos_player(zappy, &client->stats);
    add_to_buffer(&client->out_buffer, "ok\n");
}
