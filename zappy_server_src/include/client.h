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
    struct client_s *next;
    // Client information
    int fd;
    char *in_buffer;
    char *out_buffer;
    bool is_connected;
    bool is_waiting_id;
    bool is_graphic;

    // Datas game
    int id;
    int x;
    int y;
    int orientation;
    int level;
    char *team_name;

    // Inventory
    int nbr_food;
    int nbr_linemate;
    int nbr_deraumere;
    int nbr_sibur;
    int nbr_mendiane;
    int nbr_phiras;
    int nbr_thystame;
} client_t;

void add_client(zappy_t *zappy, int fd);
void remove_client(zappy_t *zappy, int fd);
void destroy_clients(client_t *clients);
void send_data(zappy_t *zappy, int fd);

#endif /* !CLIENT_H */
