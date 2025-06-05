/*
** EPITECH PROJECT, 2025
** command.h
** File description:
** Header file for command functions
*/

#ifndef COMMAND_H
    #define COMMAND_H

    #include "zappy.h"

void handle_client_command(zappy_t *zappy, int fd);
void send_client_command(zappy_t *zappy, int fd);
void append_client_out_buffer(client_t *client, const char *format, ...);
void execute_command(client_t *client, zappy_t *zappy_ptr);
void process_command(char **args, client_t *client, zappy_t *zappy_ptr);

#endif /* !COMMAND_H */
