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

typedef struct waitingCommands_s {
    struct waitingCommands_s *next;
    char **command;
    int ticksLeft;
} waitingCommands_t;

typedef struct client_s {
    struct client_s *next;
    // Client information
    int fd;
    char *in_buffer;
    char *out_buffer;
    bool is_connected;
    bool is_waiting_id;
    bool is_graphic;
    waitingCommands_t *waiting_commands;

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


void check_ticks(zappy_t *zappy);

void add_client(zappy_t *zappy, int fd);
void remove_client(zappy_t *zappy, int fd);
void destroy_clients(client_t *clients);
void send_data(zappy_t *zappy, client_t *c);
void send_data_to_graphics(zappy_t *zappy, char *data);
void add_to_buffer(char **buffer, const char *data);
int get_size(int nbr);
void push_command_to_queue(char **args, client_t *client, zappy_t *zappy_ptr);
char *realloc_strcat(char *dest, const char *src);

void send_players_data(zappy_t *zappy, client_t *c);
void send_eggs_data(zappy_t *zappy, client_t *c);
void delete_egg_team_name(zappy_t *zappy_ptr, char *team_name);

#endif /* !CLIENT_H */
