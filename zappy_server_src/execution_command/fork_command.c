/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** take_command
*/

#include <string.h>
#include "command_handler.h"

static void lay_egg(zappy_t *zappy, client_t *client)
{
    char buffer[256];
    int id_last_egg = zappy->map->eggs[0].id;
    egg_t *new_egg = malloc(sizeof(egg_t));

    new_egg->x = client->x;
    new_egg->y = client->y;
    new_egg->id = id_last_egg + 1;
    new_egg->team_name = client->team_name;
    new_egg->next = zappy->map->eggs;
    zappy->map->eggs = new_egg;
    sprintf(buffer, "enw #%d #%d %d %d", id_last_egg + 1,
        client->id, new_egg->x, new_egg->y);
    send_data_to_graphics(zappy, buffer);
}

void fork_command(zappy_t *zappy, client_t *client, char **args)
{
    (void) args;
    lay_egg(zappy, client);
    add_to_buffer(&client->out_buffer, "ok\n");
}
