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

starting_map_t *init_starting_map(zappy_t *zappy)
{
    starting_map_t *map = malloc(sizeof(starting_map_t));
    int a = 0;

    if (!map)
        return NULL;
    map->grid = create_map(zappy->parser);
    for (int i = 0; i < zappy->parser->nb_teams; ++i) {
        for (int j = 0; j < zappy->parser->clients_per_team; ++j)
            add_egg(map, a++, zappy->parser->team_names[i], zappy);
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
