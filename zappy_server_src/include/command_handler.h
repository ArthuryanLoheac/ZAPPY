/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** List/map of all zappy core command handlers 
*/

#ifndef COMMAND_HANDLER_H_
    #define COMMAND_HANDLER_H_
    #include <stddef.h>
    #include "zappy.h"
    #include "client.h"

typedef void (*command_handler_t)(zappy_t *zappy, client_t *client, char **args);

void update_pos_player(zappy_t *zappy, client_t *client);

void look_command(zappy_t *zappy, client_t *client, char **args);
void forward_command(zappy_t *zappy, client_t *client, char **args);
void right_command(zappy_t *zappy, client_t *client, char **args);
void left_command(zappy_t *zappy, client_t *client, char **args);

typedef struct command_func_s {
    char *name;
    command_handler_t func;
} command_func_t;

extern const command_func_t command_association_list[];;

command_handler_t get_command_handler(const char *name);

#endif /* !COMMAND_HANDLER_H_ */
