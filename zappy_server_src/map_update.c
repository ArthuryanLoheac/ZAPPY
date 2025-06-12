/*
** EPITECH PROJECT, 2025
** map_update.c
** File description:
** Functions to manage the update of the game map
*/

#include <stdlib.h>

#include "include/zappy.h"

static int *get_current_resources(cell_t **map, parser_t *parser)
{
    int *current_res = malloc(sizeof(int) * 7);

    if (!current_res)
        display_error("Failed to allocate memory for current resources");
    for (int i = 0; i < 7; i++)
        current_res[i] = 0;
    for (int y = 0; y < parser->height; y++) {
        for (int x = 0; x < parser->width; x++) {
            current_res[FOOD] += map[y][x].nbr_food;
            current_res[LINEMATE] += map[y][x].nbr_linemate;
            current_res[DERAUMERE] += map[y][x].nbr_deraumere;
            current_res[SIBUR] += map[y][x].nbr_sibur;
            current_res[MENDIANE] += map[y][x].nbr_mendiane;
            current_res[PHIRAS] += map[y][x].nbr_phiras;
            current_res[THYSTAME] += map[y][x].nbr_thystame;
        }
    }
    return current_res;
}

static void place_missing_resources(cell_t **map, parser_t *parser,
    int res, int missing)
{
    int total_cells = parser->width * parser->height;
    coord_t *coords = init_coord_array(parser);
    coord_t c = {0, 0};
    int pos = 0;

    for (int i = 0; i < missing; i++) {
        c = coords[pos % total_cells];
        place_resource(map, c, res);
        pos++;
    }
}

void update_map(cell_t **map, parser_t *parser)
{
    int *total_res = get_total_resources(parser);
    int *current_res = get_current_resources(map, parser);

    for (int res = FOOD; res <= THYSTAME; res++) {
        if (current_res[res] < total_res[res])
            place_missing_resources(map, parser, res,
                total_res[res] - current_res[res]);
    }
    free(total_res);
    free(current_res);
}
