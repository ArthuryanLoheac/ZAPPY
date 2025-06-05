/*
** EPITECH PROJECT, 2025
** map.c
** File description:
** Functions to manage the map of the core
*/

#include <stdlib.h>

#include "include/core.h"
#include "include/zappy.h"

static const float densities[] = {
    [FOOD] = 0.5f,
    [LINEMATE] = 0.3f,
    [DERAUMERE] = 0.15f,
    [SIBUR] = 0.1f,
    [MENDIANE] = 0.1f,
    [PHIRAS] = 0.08f,
    [THYSTAME] = 0.05f
};

static void init_cell(cell_t *cell)
{
    cell->nb_egg = 0;
    cell->nb_food = 0;
    cell->nb_linemate = 0;
    cell->nb_deraumere = 0;
    cell->nb_sibur = 0;
    cell->nb_mendiane = 0;
    cell->nb_phiras = 0;
    cell->nb_thystame = 0;
}

static int get_total_resources(parser_t *parser, float density)
{
    int total = (int)(parser->width * parser->height * density + 0.5f);

    return total < 1 ? 1 : total;
}

static void place_resources(cell_t **map, int res, int per_cell, int leftover)
{
    int *ptr = NULL;

    switch (res) {
        case FOOD:
            ptr = &map[y][x].nb_food;
            break;
        case LINEMATE:
            ptr = &map[y][x].nb_linemate;
            break;
        case DERAUMERE:
            ptr = &map[y][x].nb_deraumere;
            break;
        case SIBUR:
            ptr = &map[y][x].nb_sibur;
            break;
        case MENDIANE:
            ptr = &map[y][x].nb_mendiane;
            break;
        case PHIRAS:
            ptr = &map[y][x].nb_phiras;
            break;
        case THYSTAME:
            ptr = &map[y][x].nb_thystame;
            break;
    }
    if (ptr && per_cell > -1)
        *ptr = per_cell;
    if (ptr && leftover > -1)
        (*ptr) += leftover;
}

void place_evenly(cell_t **map, parser_t *parser, int res, int per_cell)
{
    for (int y = 0; y < parser->height; y++) {
        for (int x = 0; x < parser->width; x++)
            place_resources(map, res, per_cell, -1);
    }
}

void place_leftover(cell_t **map, parser_t *parser, int res, int leftover)
{
    int rx = 0;
    int ry = 0;

    for (int i = 0; i < leftover; i++) {
        rx = rand() % parser->width;
        ry = rand() % parser->height;
        place_resources(map, res, -1, 1);
    }
}

void place_eggs(cell_t **map, parser_t *parser, int nb_eggs)
{
    int rx = 0;
    int ry = 0;

    for (int i = 0; i < nb_eggs; i++) {
        rx = rand() % parser->width;
        ry = rand() % parser->height;
        map[ry][rx].nb_egg += 1;
    }
}

static void spawn_resources(cell_t **map, parser_t *parser)
{
    int total[7] = {0};
    int per_cell = 0;
    int leftover = 0;
    int nb_eggs = 0;

    for (int i = 0; i < 7; i++)
        total[i] = get_total_resources(parser, densities[i]);
    for (int res = 0; res < 7; res++) {
        per_cell = total[res] / (parser->width * parser->height);
        leftover = total[res] % (parser->width * parser->height);
        place_evenly(map, parser, res, per_cell);
        place_leftover(map, parser, res, leftover);
    }
    nb_eggs = parser->clients_per_team * parser->nb_teams;
    place_eggs(map, parser, nb_eggs);
}

cell_t **create_map(parser_t *parser)
{
    cell_t **map = malloc(sizeof(cell_t *) * (parser->height + 1));

    if (!map)
        display_error("Failed to allocate memory for map");
    for (int i = 0; i < parser->height; i++) {
        map[i] = malloc(sizeof(cell_t) * parser->width);
        if (!map[i])
            display_error("Failed to allocate memory for map row");
        for (int j = 0; j < parser->width; j++)
            init_cell(&map[i][j]);
    }
    map[parser->height] = NULL;
    spawn_resources(map, parser);
    return map;
}

void destroy_map(cell_t **map)
{
    if (!map)
        return;
    for (int i = 0; map[i]; i++)
        free(map[i]);
    free(map);
}
