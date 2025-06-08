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

static void init_cell(cell_t *cell, int y, int x)
{
    cell->coord.x = x;
    cell->coord.y = y;
    cell->nb_egg = 0;
    cell->nb_food = 0;
    cell->nb_linemate = 0;
    cell->nb_deraumere = 0;
    cell->nb_sibur = 0;
    cell->nb_mendiane = 0;
    cell->nb_phiras = 0;
    cell->nb_thystame = 0;
    cell->nb_players = 0;
}

static void shuffle_coords(coord_t *coords, int total_cell)
{
    int j = 0;
    coord_t temp;

    for (int i = total_cell - 1; i > 0; i--) {
        j = rand() % (i + 1);
        temp = coords[i];
        coords[i] = coords[j];
        coords[j] = temp;
    }
}

static coord_t *init_coord_array(parser_t *parser)
{
    int total_cells = parser->width * parser->height;
    coord_t *coords = malloc(sizeof(coord_t) * total_cells);
    int pos = 0;

    if (!coords)
        display_error("Failed to allocate memory for coordinates");
    for (int y = 0; y < parser->height; y++) {
        for (int x = 0; x < parser->width; x++) {
            coords[pos].x = x;
            coords[pos].y = y;
            pos++;
        }
    }
    shuffle_coords(coords, total_cells);
    return coords;
}

static int get_total_resources(parser_t *parser, float density)
{
    int total = (int)(parser->width * parser->height * density + 0.5f);

    return total < 1 ? 1 : total;
}

void place_resource(cell_t **map, coord_t c, int res)
{
    if (res == FOOD)
        map[c.y][c.x].nb_food += 1;
    if (res == LINEMATE)
        map[c.y][c.x].nb_linemate += 1;
    if (res == DERAUMERE)
        map[c.y][c.x].nb_deraumere += 1;
    if (res == SIBUR)
        map[c.y][c.x].nb_sibur += 1;
    if (res == MENDIANE)
        map[c.y][c.x].nb_mendiane += 1;
    if (res == PHIRAS)
        map[c.y][c.x].nb_phiras += 1;
    if (res == THYSTAME)
        map[c.y][c.x].nb_thystame += 1;
}

void place_eggs(cell_t **map, parser_t *parser)
{
    int nb_eggs = parser->clients_per_team * parser->nb_teams;
    int rx = 0;
    int ry = 0;

    for (int i = 0; i < nb_eggs; i++) {
        rx = rand() % parser->width;
        ry = rand() % parser->height;
        map[ry][rx].nb_egg += 1;
    }
}

void spawn_resources(cell_t **map, parser_t *parser)
{
    int total_cells = parser->width * parser->height;
    coord_t *coords = init_coord_array(parser);
    int total_res[7] = {0};
    coord_t c = {0, 0};
    int pos = 0;

    for (int i = 0; i < 7; i++)
        total_res[i] = get_total_resources(parser, densities[i]);
    for (int res = FOOD; res <= THYSTAME; res++) {
        for (int j = 0; j < total_res[res]; j++) {
            c = coords[pos % total_cells];
            place_resource(map, c, res);
            pos++;
        }
    }
    free(coords);
    place_eggs(map, parser);
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
