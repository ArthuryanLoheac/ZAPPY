/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** right_command
*/

#include "command_handler.h"

void right_command(zappy_t *zappy, client_t *client, char **args)
{
    (void) args;
    client->stats.orientation++;
    if (client->stats.orientation > 4)
        client->stats.orientation = 1;
    update_pos_player(zappy, &client->stats);
    add_to_buffer(&client->out_buffer, "ok\n");
}
