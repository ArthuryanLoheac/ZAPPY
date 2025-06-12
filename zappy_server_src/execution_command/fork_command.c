/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** take_command
*/

#include <string.h>
#include "command_handler.h"

void fork_command(zappy_t *zappy, client_t *client, char **args)
{
    (void) args;
    lay_egg(zappy, client);
    add_to_buffer(&client->out_buffer, "ok\n");
}
