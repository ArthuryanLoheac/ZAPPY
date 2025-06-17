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
#include "logs.h"

static const float densities[] = {
    [FOOD] = 0.5f,
    [LINEMATE] = 0.3f,
    [DERAUMERE] = 0.15f,
    [SIBUR] = 0.1f,
    [MENDIANE] = 0.1f,
    [PHIRAS] = 0.08f,
    [THYSTAME] = 0.05f
};

int *get_total_resources(parser_t *parser)
{
    int *result = malloc(sizeof(int) * 7);

    if (!result)
        display_error("Failed to allocate memory for total resources");
    for (int i = 0; i < 7; i++)
        result[i] = (int)(parser->width * parser->height *
            densities[i] + 0.5f);
    return result;
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

coord_t *init_coord_array(parser_t *parser)
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

void place_resource(cell_t **map, coord_t c, int res)
{
    if (res == FOOD)
        map[c.y][c.x].nbr_food += 1;
    if (res == LINEMATE)
        map[c.y][c.x].nbr_linemate += 1;
    if (res == DERAUMERE)
        map[c.y][c.x].nbr_deraumere += 1;
    if (res == SIBUR)
        map[c.y][c.x].nbr_sibur += 1;
    if (res == MENDIANE)
        map[c.y][c.x].nbr_mendiane += 1;
    if (res == PHIRAS)
        map[c.y][c.x].nbr_phiras += 1;
    if (res == THYSTAME)
        map[c.y][c.x].nbr_thystame += 1;
}

static void add_egg(starting_map_t *map, const char *team_name,
    zappy_t *zappy)
{
    egg_t *new_egg = malloc(sizeof(egg_t));

    if (!new_egg)
        return;
    new_egg->x = rand() % zappy->parser->width;
    new_egg->y = rand() % zappy->parser->height;
    new_egg->id = map->id_egg;
    map->id_egg += 1;
    new_egg->team_name = strdup(team_name);
    new_egg->next = map->eggs;
    map->eggs = new_egg;
}

starting_map_t *init_starting_map(zappy_t *zappy)
{
    starting_map_t *map = malloc(sizeof(starting_map_t));

    map->id_egg = 0;
    LOG_DEBUG("Initializing map");
    if (!map)
        return NULL;
    map->grid = create_map(zappy->parser);
    for (int i = 0; i < zappy->parser->nb_teams; ++i) {
        for (int j = 0; j < zappy->parser->clients_per_team; ++j)
            add_egg(map, zappy->parser->team_names[i], zappy);
    }
    return map;
}

void free_starting_map(starting_map_t *map)
{
    if (!map)
        return;
    destroy_map(map->grid);
    if (map->eggs)
        free(map->eggs);
    free(map);
}
