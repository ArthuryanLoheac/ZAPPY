/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** team name
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "logs.h"

#include "command_handler.h"


static void move_player(zappy_t *zappy, stats_t *stats)
{
    if (stats->orientation == 1)
        stats->y = (stats->y + 1) % zappy->parser->height;
    if (stats->orientation == 4)
        stats->x = (stats->x + 1) % zappy->parser->width;
    if (stats->orientation == 3) {
        stats->y -= 1;
        if (stats->y < 0)
            stats->y = zappy->parser->height - 1;
    }
    if (stats->orientation == 2) {
        stats->x -= 1;
        if (stats->x < 0)
            stats->x = zappy->parser->width - 1;
    }
}

static client_t *get_client(int id, zappy_t *zappy)
{
    client_t *c = zappy->clients;

    while (c != NULL) {
        if (c->stats.id == id)
            return c;
        c = c->next;
    }
    return NULL;
}

void d_backward_command(zappy_t *zappy, client_t *client, char **args)
{
    stats_t *stats;
    client_t *target_client = NULL;
    char buffer[256];
    int id = -1;

    if (client == NULL || zappy == NULL || args == NULL || args[0] == NULL ||
        strlen(args[0]) < 2)
        return;
    id = atoi(args[0] + 1);
    target_client = get_client(id, zappy);
    if (target_client == NULL) {
        add_to_buffer(&client->out_buffer, "ko\n");
        return;
    }
    stats = &target_client->stats;
    move_player(zappy, stats);
    update_pos_player(zappy, &target_client->stats);
    snprintf(buffer, sizeof(buffer), "ppo #%d %d %d %d\n",
        stats->id, stats->x, stats->y, stats->orientation);
    send_data_to_graphics(zappy, buffer);
}
