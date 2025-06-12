/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Get time units per seconds
*/

#include <stdio.h>

#include "command_handler.h"

void sgt_command(zappy_t *zappy, client_t *client, char **args)
{
    char response[20];

    (void) args;
    snprintf(response, 19, "sgt %i\n", zappy->parser->freq);
    add_to_buffer(&client->out_buffer, response);
}
