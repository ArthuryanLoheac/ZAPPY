/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** right_command
*/

#include "command_handler.h"

void left_command(zappy_t *zappy, client_t *client, char **args)
{
    (void) args;
    client->stats.orientation--;
    if (client->stats.orientation < 1)
        client->stats.orientation = 4;
    update_pos_player(zappy, client);
    add_to_buffer(&client->out_buffer, "ok\n");
}
