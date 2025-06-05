/*
** EPITECH PROJECT, 2025
** map.h
** File description:
** Header file for the map functions
*/

#ifndef MAP_H
    #define MAP_H

    #include "parser.h"

enum resource_type {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    EGG
};

typedef struct cell_s {
    int nb_egg;
    int nb_food;
    int nb_linemate;
    int nb_deraumere;
    int nb_sibur;
    int nb_mendiane;
    int nb_phiras;
    int nb_thystame;
} cell_t;

cell_t **create_map(parser_t *parser);
void destroy_map(cell_t **map);

#endif /* !MAP_H */
