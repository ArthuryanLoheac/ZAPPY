/*
** EPITECH PROJECT, 2025
** core.h
** File description:
** Header file for the core functions
*/

#ifndef CORE_H
    #define CORE_H

    #include "zappy.h"
    #include "map.h"

typedef struct core_s {
    cell_t **map;
} core_t;

core_t *init_core(zappy_t *zappy);
void destroy_core(core_t *core);

#endif /* !CORE_H */
