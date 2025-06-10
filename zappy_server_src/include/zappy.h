/*
** EPITECH PROJECT, 2025
** zappy.h
** File description:
** Header file for the Zappy server
*/

#ifndef ZAPPY_H
    #define ZAPPY_H

    #include <sys/time.h>
    #include "parser.h"
    #include "server.h"

typedef struct client_s client_t;

enum resource_type {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    EGG,
};

typedef struct coord_s {
    int x;
    int y;
} coord_t;

typedef struct cell_s {
    int x;
    int y;

    int nbr_food;
    int nbr_linemate;
    int nbr_deraumere;
    int nbr_sibur;
    int nbr_mendiane;
    int nbr_phiras;
    int nbr_thystame;
} cell_t;

typedef struct egg_s {
    int x;
    int y;
    int id;
    char *team_name;
    struct egg_s *next;
} egg_t;

typedef struct starting_map_s {
    cell_t **grid;
    egg_t *eggs;
} starting_map_t;

typedef struct zappy_s {
    parser_t *parser;
    parser_str_t *parser_str;
    server_t *server;
    client_t *clients;
    starting_map_t *map;
    int idNextClient;
    float durationTickLeft;
    float durationTick;

    struct timeval last_time;
} zappy_t;

typedef void (*incr_func_t)(cell_t *);

void incr_egg(cell_t *cell);
void incr_food(cell_t *cell);
void incr_linemate(cell_t *cell);
void incr_deraumere(cell_t *cell);
void incr_sibur(cell_t *cell);
void incr_mendiane(cell_t *cell);
void incr_phiras(cell_t *cell);
void incr_thystame(cell_t *cell);

starting_map_t *init_starting_map(zappy_t *zappy);
void free_starting_map(starting_map_t *map);
cell_t **create_map(parser_t *parser);
void destroy_map(cell_t **map);

void start_server(zappy_t *zappy);
void down_server(zappy_t *zappy);

#endif /* !ZAPPY_H */
