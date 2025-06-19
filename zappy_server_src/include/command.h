/*
** EPITECH PROJECT, 2025
** command.h
** File description:
** Header file for command functions
*/

#ifndef COMMAND_H
    #define COMMAND_H

    #include "zappy.h"
    #include "client.h"

typedef struct command_name_size_s {
    const char *name;
    unsigned int duration;
} command_name_size_t;

typedef struct required_elevation_s {
    unsigned int nb_player;
    unsigned int nb_linemate;
    unsigned int nb_deraumere;
    unsigned int nb_sibur;
    unsigned int nb_mendiane;
    unsigned int nb_phiras;
    unsigned int nb_thystame;
} required_elevation_t;



extern const command_name_size_t player_command_name_size_list[];
extern const command_name_size_t gui_command_name_size_list[];
extern const required_elevation_t requires_elevation_list[];

int get_gui_command_duration(const char *name);
int get_player_command_duration(const char *name);

void handle_client_command(zappy_t *zappy, int fd);
void send_client_command(zappy_t *zappy, int fd);
void append_client_out_buffer(client_t *client, const char *format, ...);
void extract_commands(client_t *client, zappy_t *zappy_ptr);
void process_command(char **args, client_t *client, zappy_t *zappy_ptr);

#endif /* !COMMAND_H */
