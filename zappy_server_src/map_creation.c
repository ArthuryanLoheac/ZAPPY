/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** map_creation
*/

#include <stdio.h>
#include <stdlib.h>

#include "include/zappy.h"

static void distr_res(cell_t **grid, parser_t *parser,
    int amount, incr_func_t incr)
{
    int x = 0;
    int y = 0;

    for (int i = 0; i < amount; ++i) {
        x = rand() % parser->width;
        y = rand() % parser->height;
        incr(&grid[y][x]);
    }
}

static void do_distrib(starting_map_t *map, zappy_t *zappy, int num_teams)
{
    int grid_size = zappy->parser->width * zappy->parser->height;

    distr_res(map->grid, zappy->parser, 3 * num_teams, incr_egg);
    distr_res(map->grid, zappy->parser, grid_size * 0.5, incr_food);
    distr_res(map->grid, zappy->parser, grid_size * 0.3, incr_linemate);
    distr_res(map->grid, zappy->parser, grid_size * 0.15, incr_deraumere);
    distr_res(map->grid, zappy->parser, grid_size * 0.1, incr_sibur);
    distr_res(map->grid, zappy->parser, grid_size * 0.1, incr_mendiane);
    distr_res(map->grid, zappy->parser, grid_size * 0.08, incr_phiras);
    distr_res(map->grid, zappy->parser, grid_size * 0.05, incr_thystame);
}

starting_map_t *init_starting_map(zappy_t *zappy, int num_teams)
{
    starting_map_t *map = malloc(sizeof(starting_map_t));

    if (!map)
        return NULL;
    map->grid = malloc(sizeof(cell_t *) * zappy->parser->height);
    if (!map->grid)
        return NULL;
    for (int y = 0; y < zappy->parser->height; ++y) {
        map->grid[y] = calloc(zappy->parser->width, sizeof(cell_t));
        if (!map->grid[y])
            return NULL;
    }
    do_distrib(map, zappy, num_teams);
    return (map);
}

void free_starting_map(starting_map_t *map, int height)
{
    if (!map)
        return;
    for (int y = 0; y < height; ++y)
        free(map->grid[y]);
    free(map->grid);
    free(map);
}
