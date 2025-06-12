/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Utility functions used by multiple comands handlers
*/

#include <stdio.h>
#include <stdlib.h>

#include "command_handler.h"

void update_pos_player(zappy_t *zappy, client_t *client)
{
    char buffer[256];

    sprintf(buffer, "ppo #%d %d %d %d\n", client->id, client->x, client->y,
        client->orientation);
    send_data_to_graphics(zappy, buffer);
}

void update_cell(zappy_t *zappy, cell_t cell)
{
    char buffer[256];

    sprintf(buffer, "bct %d %d %d %d %d %d %d %d %d\n",
        cell.x, cell.y, cell.nbr_food,
        cell.nbr_linemate, cell.nbr_deraumere, cell.nbr_sibur,
        cell.nbr_mendiane, cell.nbr_phiras, cell.nbr_thystame);
    send_data_to_graphics(zappy, buffer);
}

void lay_egg(zappy_t *zappy, client_t *client)
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
