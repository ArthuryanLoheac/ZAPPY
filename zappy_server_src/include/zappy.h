/*
** EPITECH PROJECT, 2025
** zappy.h
** File description:
** Header file for the Zappy server
*/

#ifndef ZAPPY_H
    #define ZAPPY_H

typedef struct parser_s parser_t;

typedef struct server_s server_t;

typedef struct zappy_s {
    parser_t *parser;
    server_t *server;
} zappy_t;

void start_server(zappy_t *zappy);
void down_server(zappy_t *zappy);

#endif /* !ZAPPY_H */
