/*
** EPITECH PROJECT, 2025
** map.c
** File description:
** Functions to manage the map of the core
*/

#include <stdlib.h>

#include "include/zappy.h"

static void init_cell(cell_t *cell, int y, int x)
{
    cell->x = x;
    cell->y = y;
    cell->nbr_food = 0;
    cell->nbr_linemate = 0;
    cell->nbr_deraumere = 0;
    cell->nbr_sibur = 0;
    cell->nbr_mendiane = 0;
    cell->nbr_phiras = 0;
    cell->nbr_thystame = 0;
}

void spawn_resources(cell_t **map, parser_t *parser)
{
    int total_cells = parser->width * parser->height;
    coord_t *coords = init_coord_array(parser);
    int *total_res = get_total_resources(parser);
    coord_t c = {0, 0};
    int pos = 0;

    for (int res = FOOD; res <= THYSTAME; res++) {
        for (int j = 0; j < total_res[res]; j++) {
            c = coords[pos % total_cells];
            place_resource(map, c, res);
            pos++;
        }
    }
    free(coords);
    free(total_res);
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
            init_cell(&map[i][j], i, j);
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
