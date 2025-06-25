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

static int get_id_team(zappy_t *zappy, const char *team_name)
{
    int id = 0;

    if (zappy == NULL || team_name == NULL)
        return -1;
    for (int i = 0; i < zappy->parser->nb_teams; i++) {
        if (strcmp(zappy->parser->team_names[i], team_name) == 0) {
            id = i;
            break;
        }
    }
    return id;
}

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
        get_id_team(zappy, client->stats.team_name), new_egg->x, new_egg->y);
    send_data_to_graphics(zappy, buffer);
}

static client_t *get_client(zappy_t *zappy, char **args)
{
    client_t *c = zappy->clients;
    int id = -1;

    if (zappy == NULL || !args || args[0] == NULL || strlen(args[0]) < 2)
        return NULL;
    id = atoi(args[0] + 1);
    if (id < 0)
        return NULL;
    while (c != NULL) {
        if (c->stats.id == id)
            return c;
        c = c->next;
    }
    return NULL;
}

void d_fork_command(zappy_t *zappy, client_t *client, char **args)
{
    client_t *target = NULL;
    char buffer[256];

    if (client == NULL || zappy == NULL || args == NULL || args[0] == NULL)
        return;
    target = get_client(zappy, args);
    if (target == NULL) {
        add_to_buffer(&client->out_buffer, "ko\n");
        return;
    }
    lay_egg(zappy, target);
    sprintf(buffer, "pfk #%d\n", target->stats.id);
    send_data_to_graphics(zappy, buffer);
}
