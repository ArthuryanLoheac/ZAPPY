/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Map size
*/

#include <stdio.h>

#include "command.h"

void msz_command(zappy_t *zappy, client_t *client, char **args)
{
    char response[20];

    (void) args;
    snprintf(response, 20, "msz %i %i\n",
        zappy->parser->width, zappy->parser->height);
    add_to_buffer(&client->out_buffer, response);
}
