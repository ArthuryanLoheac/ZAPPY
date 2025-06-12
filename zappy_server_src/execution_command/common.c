/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Utility functions used by multiple commands handlers
*/

#include <stdio.h>

#include "command_handler.h"

void update_pos_player(zappy_t *zappy, stats_t *stat)
{
    char buffer[256];

    sprintf(buffer, "ppo #%d %d %d %d\n", stat->id, stat->x, stat->y,
        stat->orientation);
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

void send_bloc_content(int x, int y, zappy_t *zappy, client_t *client)
{
    char response[100];

    snprintf(response, 99, "bct %i %i %i %d %d %d %i %i %i\n",
        x,
        y,
        zappy->map->grid[y][x].nbr_food,
        zappy->map->grid[y][x].nbr_linemate,
        zappy->map->grid[y][x].nbr_deraumere,
        zappy->map->grid[y][x].nbr_sibur,
        zappy->map->grid[y][x].nbr_mendiane,
        zappy->map->grid[y][x].nbr_phiras,
        zappy->map->grid[y][x].nbr_thystame
    );
    add_to_buffer(&client->out_buffer, response);
}
