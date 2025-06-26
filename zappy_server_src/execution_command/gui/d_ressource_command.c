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

static void send_ressource_update(zappy_t *zappy, client_t *client, cell_t *cell)
{
    char buffer[256];

    if (cell == NULL || zappy == NULL || client == NULL)
        return;
    sprintf(buffer, "bct %d %d %d %d %d %d %d %d %d\n",
        cell->x, cell->y, cell->nbr_food, cell->nbr_linemate,
        cell->nbr_deraumere, cell->nbr_sibur, cell->nbr_mendiane,
        cell->nbr_phiras, cell->nbr_thystame);
    send_data_to_graphics(zappy, buffer);
}

static int min_quantity(int original, int add)
{
    original += add;
    if (original < 0)
        original = 0;
    return original;
}

static void update_cell_command(cell_t *cell, const char *resource, int quantity,
client_t *client)
{
    if (strcmp(resource, "food") == 0) {
        cell->nbr_food = min_quantity(cell->nbr_food, quantity);
    } else if (strcmp(resource, "linemate") == 0) {
        cell->nbr_linemate = min_quantity(cell->nbr_linemate, quantity);
    } else if (strcmp(resource, "deraumere") == 0) {
        cell->nbr_deraumere = min_quantity(cell->nbr_deraumere, quantity);
    } else if (strcmp(resource, "sibur") == 0) {
        cell->nbr_sibur = min_quantity(cell->nbr_sibur, quantity);
    } else if (strcmp(resource, "mendiane") == 0) {
        cell->nbr_mendiane = min_quantity(cell->nbr_mendiane, quantity);
    } else if (strcmp(resource, "phiras") == 0) {
        cell->nbr_phiras = min_quantity(cell->nbr_phiras, quantity);
    } else if (strcmp(resource, "thystame") == 0) {
        cell->nbr_thystame = min_quantity(cell->nbr_thystame, quantity);
    } else {
        add_to_buffer(&client->out_buffer, "ko\n");
        return;
    }
}

static cell_t *get_cell(zappy_t *zappy, int x, int y)
{
    if (zappy == NULL || zappy->map == NULL)
        return NULL;
    for (int i = 0; i < zappy->parser->width; i++) {
        for (int j = 0; j < zappy->parser->height; j++) {
            if (zappy->map->grid[i][j].x == x && zappy->map->grid[i][j].y == y)
                return &zappy->map->grid[i][j];
        }
    }
    return NULL;
}

void d_ressource_command(zappy_t *zappy, client_t *client, char **args)
{
    int x;
    int y;
    char *resource;
    int quantity;

    if (client == NULL || zappy == NULL || args == NULL ||
        !args[0] || !args[1] || !args[2] || !args[3])
        return;
    x = atoi(args[0]);
    y = atoi(args[1]);
    resource = args[2];
    quantity = atoi(args[3]);
    if (x < 0 || y < 0) {
        add_to_buffer(&client->out_buffer, "ko\n");
        return;
    }
    cell_t *cell = get_cell(zappy, x, y);
    if (cell == NULL) {
        add_to_buffer(&client->out_buffer, "ko\n");
        return;
    }
    update_cell_command(cell, resource, quantity, client);
    send_ressource_update(zappy, client, cell);
}
