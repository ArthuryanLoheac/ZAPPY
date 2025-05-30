/*
** EPITECH PROJECT, 2025
** client.h
** File description:
** Header file for client functions
*/

#ifndef CLIENT_H
    #define CLIENT_H

    #include <stdbool.h>

    #include "zappy.h"

typedef struct client_s {
    int fd;
    char *in_buffer;
    char *out_buffer;
    bool is_connected;
    struct client_s *next;
} client_t;

void add_client(zappy_t *zappy, int fd);
void remove_client(zappy_t *zappy, int fd);
void destroy_clients(client_t *clients);

#endif /* !CLIENT_H */
