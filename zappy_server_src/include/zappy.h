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

    unsigned int nbr_food;
    unsigned int nbr_linemate;
    unsigned int nbr_deraumere;
    unsigned int nbr_sibur;
    unsigned int nbr_mendiane;
    unsigned int nbr_phiras;
    unsigned int nbr_thystame;
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
    int id_egg;
} starting_map_t;

typedef struct pos_elevation_s {
    int x;
    int y;
    int level;
    struct pos_elevation_s *next;
} pos_elevation_t;


typedef struct zappy_s {
    parser_t *parser;
    parser_str_t *parser_str;
    server_t *server;
    client_t *clients;
    starting_map_t *map;
    int idNextClient;
    float durationTickLeft;
    float durationTick;
    int tickCount;

    struct timeval last_time;
    pos_elevation_t *pos_elevations;
    pos_elevation_t *pos_elevationsFail;
} zappy_t;

starting_map_t *init_starting_map(zappy_t *zappy);
int *get_total_resources(parser_t *parser);
coord_t *init_coord_array(parser_t *parser);
void place_resource(cell_t **map, coord_t c, int res);
void free_starting_map(starting_map_t *map);
cell_t **create_map(parser_t *parser);
void update_map(cell_t **map, parser_t *parser);
void destroy_map(cell_t **map);

void start_server(zappy_t *zappy);
void down_server(zappy_t *zappy);

#endif /* !ZAPPY_H */
