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

static void send_ressource_update(zappy_t *zappy, client_t *client,
    cell_t *cell)
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

static void update_cell_command(cell_t *cell, const char *resource,
    int quantity)
{
    if (strcmp(resource, "food") == 0)
        cell->nbr_food = min_quantity(cell->nbr_food, quantity);
    if (strcmp(resource, "linemate") == 0)
        cell->nbr_linemate = min_quantity(cell->nbr_linemate, quantity);
    if (strcmp(resource, "deraumere") == 0)
        cell->nbr_deraumere = min_quantity(cell->nbr_deraumere, quantity);
    if (strcmp(resource, "sibur") == 0)
        cell->nbr_sibur = min_quantity(cell->nbr_sibur, quantity);
    if (strcmp(resource, "mendiane") == 0)
        cell->nbr_mendiane = min_quantity(cell->nbr_mendiane, quantity);
    if (strcmp(resource, "phiras") == 0)
        cell->nbr_phiras = min_quantity(cell->nbr_phiras, quantity);
    if (strcmp(resource, "thystame") == 0)
        cell->nbr_thystame = min_quantity(cell->nbr_thystame, quantity);
}

static cell_t *check_line(zappy_t *zappy, int x, int y, int i)
{
    for (int j = 0; j < zappy->parser->height; j++) {
        if (zappy->map->grid[i][j].x == x && zappy->map->grid[i][j].y == y)
            return &zappy->map->grid[i][j];
    }
    return NULL;
}

static cell_t *get_cell(zappy_t *zappy, int x, int y)
{
    if (zappy == NULL || zappy->map == NULL)
        return NULL;
    for (int i = 0; i < zappy->parser->width; i++) {
        if (check_line(zappy, x, y, i) != NULL)
            return check_line(zappy, x, y, i);
    }
    return NULL;
}

int check_args(zappy_t *zappy, client_t *client, char **args)
{
    int nb_args = 0;

    while (args[nb_args] != NULL)
        nb_args++;
    if (client == NULL || zappy == NULL || args == NULL || nb_args != 4) {
        add_to_buffer(&client->out_buffer, "ko\n");
        return 1;
    }
    return 0;
}

void d_ressource_command(zappy_t *zappy, client_t *client, char **args)
{
    int x;
    int y;
    int quantity;
    char *resource = NULL;
    cell_t *cell = NULL;

    if (check_args(zappy, client, args) != 0)
        return;
    x = atoi(args[0]);
    y = atoi(args[1]);
    resource = strdup(args[2]);
    quantity = atoi(args[3]);
    if (x < 0 || y < 0 || resource == NULL)
        return;
    cell = get_cell(zappy, x, y);
    if (cell == NULL)
        return;
    update_cell_command(cell, resource, quantity);
    send_ressource_update(zappy, client, cell);
    free(resource);
}
