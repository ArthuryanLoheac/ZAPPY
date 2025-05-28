/*
** EPITECH PROJECT, 2025
** zappy.h
** File description:
** Header file for the Zappy server
*/

#ifndef ZAPPY_H
    #define ZAPPY_H

#include "parser.h"
#include "server.h"

typedef struct client_s client_t;

typedef struct zappy_s {
    parser_t *parser;
    server_t *server;
    client_t *clients;
} zappy_t;

void start_server(zappy_t *zappy);
void down_server(zappy_t *zappy);

#endif /* !ZAPPY_H */
