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

static void lay_egg(zappy_t *zappy, client_t *client)
{
    char buffer[256];
    egg_t *new_egg = malloc(sizeof(egg_t));

    new_egg->x = client->stats.x;
    new_egg->y = client->stats.y;
    new_egg->id = zappy->map->id_egg;
    new_egg->team_name = strdup(client->stats.team_name);
    new_egg->next = zappy->map->eggs;
    zappy->map->eggs = new_egg;
    zappy->map->id_egg += 1;
    sprintf(buffer, "enw #%d #%d %d %d\n", zappy->map->id_egg,
        client->stats.id, new_egg->x, new_egg->y);
    send_data_to_graphics(zappy, buffer);
}

void fork_command(zappy_t *zappy, client_t *client, char **args)
{
    if (client == NULL || zappy == NULL)
        return;
    (void) args;
    lay_egg(zappy, client);
    add_to_buffer(&client->out_buffer, "ok\n");
}
