/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** bct_command
*/

#include <stdlib.h>
#include <stdio.h>

#include "command_handler.h"
#include "pointlen.h"
#include "logs.h"

static void send_bloc_content(int x, int y, zappy_t *zappy, client_t *client)
{
    char response[100];

    snprintf(response, 100, "bct %i %i %i %d %d %d %i %i %i\n",
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

void bct_command(zappy_t *zappy, client_t *client, char **args)
{
    if (pointlen(args) != 3) {
        add_to_buffer(&client->out_buffer, "ko\n");
        LOG_WARNING("[%i]: Wrong number of args for a bct command",
            client->fd);
    } else {
        send_bloc_content( atoi(args[1]), atoi(args[2]),
            zappy, client);
    }
}
