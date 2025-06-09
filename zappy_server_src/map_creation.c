/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** map_creation
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

static void add_egg(starting_map_t *map, int id, const char *team_name,
    zappy_t *zappy)
{
    egg_t *new_egg = malloc(sizeof(egg_t));

    if (!new_egg)
        return;
    new_egg->x = rand() % zappy->parser->width;
    new_egg->y = rand() % zappy->parser->height;
    new_egg->id = id;
    new_egg->team_name = strdup(team_name);
    new_egg->next = map->eggs;
    map->eggs = new_egg;
}

static void do_distrib(starting_map_t *map, zappy_t *zappy, int num_teams)
{
    int grid_size = zappy->parser->width * zappy->parser->height;
    int a = 0;

    map->eggs = NULL;
    for (int i = 0; i < num_teams; ++i) {
        for (int j = 0; j < zappy->parser->clients_per_team; ++j)
            add_egg(map, a++, zappy->parser->team_names[i], zappy);
    }
    distr_res(map->grid, zappy->parser, grid_size * 0.5, incr_food);
    distr_res(map->grid, zappy->parser, grid_size * 0.3, incr_linemate);
    distr_res(map->grid, zappy->parser, grid_size * 0.15, incr_deraumere);
    distr_res(map->grid, zappy->parser, grid_size * 0.1, incr_sibur);
    distr_res(map->grid, zappy->parser, grid_size * 0.1, incr_mendiane);
    distr_res(map->grid, zappy->parser, grid_size * 0.08, incr_phiras);
    distr_res(map->grid, zappy->parser, grid_size * 0.05, incr_thystame);
}

static void init_line(zappy_t *zappy, starting_map_t *map, int y)
{
    for (int x = 0; x < zappy->parser->width; ++x) {
        map->grid[y][x].x = y;
        map->grid[y][x].y = x;
    }
}

starting_map_t *init_starting_map(zappy_t *zappy, int num_teams)
{
    starting_map_t *map = malloc(sizeof(starting_map_t));

    if (!map)
        return NULL;
    map->grid = malloc(sizeof(cell_t *) * zappy->parser->height);
    if (!map->grid) {
        free(map);
        return NULL;
    }
    for (int y = 0; y < zappy->parser->height; ++y) {
        map->grid[y] = calloc(zappy->parser->width, sizeof(cell_t));
        if (!map->grid[y]) {
            free_starting_map(map, y);
            return NULL;
        }
        init_line(zappy, map, y);
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
    if (map->eggs)
        free(map->eggs);
    free(map);
}
