/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** cheats command
*/

#include "command_handler.h"
#include <stdio.h>

void max_level_command(zappy_t *zappy, client_t *client, char **args)
{
    char buffer[256];
    char buffer2[256];

    (void) args;
    client->stats.level = 8;
    sprintf(buffer, "Current level: 8\n");
    add_to_buffer(&client->out_buffer, buffer);
    sprintf(buffer2, "plv #%d 8\n", client->stats.id);
    send_data_to_graphics(zappy, buffer2);
    check_win(zappy);
}
