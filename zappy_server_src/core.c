/*
** EPITECH PROJECT, 2025
** core.c
** File description:
** Functions to manage the core of the Zappy server
*/

#include <stdlib.h>

#include "include/core.h"
#include "include/zappy.h"

core_t *init_core(zappy_t *zappy)
{
    core_t *core = malloc(sizeof(core_t));

    if (!core)
        display_error("Failed to allocate memory for core");
    core->map = create_map(zappy->parser);
    return core;
}

void destroy_core(core_t *core)
{
    destroy_map(core->map);
    free(core);
}
