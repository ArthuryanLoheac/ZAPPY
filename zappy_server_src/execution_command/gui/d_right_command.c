/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** right_command
*/
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "logs.h"

#include "command_handler.h"

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

void d_right_command(zappy_t *zappy, client_t *client, char **args)
{
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
    target_client->stats.orientation--;
    if (target_client->stats.orientation < 1)
        target_client->stats.orientation = 4;
    update_pos_player(zappy, &target_client->stats);
    snprintf(buffer, sizeof(buffer), "ppo #%d %d %d %d\n",
        target_client->stats.id, target_client->stats.x,
        target_client->stats.y, target_client->stats.orientation);
    send_data_to_graphics(zappy, buffer);
}