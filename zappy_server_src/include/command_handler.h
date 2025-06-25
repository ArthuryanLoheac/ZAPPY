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

typedef struct look_variable_s {
    int level;
    int x;
    int y;
} look_variable_t;

typedef void (*command_handler_t)
(zappy_t *zappy, client_t *client, char **args);

// Commonly used functions
void update_pos_player(zappy_t *zappy, stats_t *stat);
void update_cell(zappy_t *zappy, cell_t cell);
void send_bloc_content(int x, int y, zappy_t *zappy, client_t *client);

// Player commands
void look_command(zappy_t *zappy, client_t *client, char **args);
void forward_command(zappy_t *zappy, client_t *client, char **args);
void right_command(zappy_t *zappy, client_t *client, char **args);
void left_command(zappy_t *zappy, client_t *client, char **args);
void take_command(zappy_t *zappy, client_t *client, char **args);
void set_command(zappy_t *zappy, client_t *client, char **args);
void connect_nbr_command(zappy_t *zappy, client_t *client, char **args);
void fork_command(zappy_t *zappy, client_t *client, char **args);
void start_incantation_command(zappy_t *zappy, client_t *client, char **args);
void incantation_command(zappy_t *zappy, client_t *client, char **args);
void eject_command(zappy_t *zappy, client_t *client, char **args);
void broadcast_command(zappy_t *zappy, client_t *client, char **args);
void inventory_command(zappy_t *zappy, client_t *client, char **args);

// GUI commands
void msz_command(zappy_t *zappy, client_t *client, char **args);
void bct_command(zappy_t *zappy, client_t *client, char **args);
void mct_command(zappy_t *zappy, client_t *client, char **args);
void tna_command(zappy_t *zappy, client_t *client, char **args);
void ppo_command(zappy_t *zappy, client_t *client, char **args);
void plv_command(zappy_t *zappy, client_t *client, char **args);
void pin_command(zappy_t *zappy, client_t *client, char **args);
void sgt_command(zappy_t *zappy, client_t *client, char **args);
void sst_command(zappy_t *zappy, client_t *client, char **args);
void levelup_command(zappy_t *zappy, client_t *client, char **args);

typedef struct command_func_s {
    char *name;
    command_handler_t func;
} command_func_t;

extern const command_func_t player_command_association_list[];
extern const command_func_t gui_command_association_list[];

command_handler_t get_player_command_handler(const char *name);
command_handler_t get_gui_command_handler(const char *name);

#endif /* !COMMAND_HANDLER_H_ */
