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

typedef struct inventory_s {
    int food;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} inventory_t;

typedef struct stats_s {
    int id;
    int x;
    int y;
    int level;
    int orientation;
    int tickLife;
    char *team_name;
    inventory_t inventory;
    int tickRemainingElevation;
} stats_t;

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

    // Game datas
    stats_t stats;
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

void send_players_data(zappy_t *zappy, client_t *c);
void send_eggs_data(zappy_t *zappy, client_t *c);
void delete_egg_team_name(zappy_t *zappy_ptr, char *team_name);

int handle_incantation(char **args, zappy_t *zappy_ptr, client_t *client);
void consume_incantation(zappy_t *zappy_ptr, int x, int y, int level);
int check_incantation_valid(zappy_t *zappy_ptr, client_t *client, int level);
bool add_command(int duration, char **args, client_t *client);
bool add_command_second(int duration, char **args, client_t *client);

#endif /* !CLIENT_H */
