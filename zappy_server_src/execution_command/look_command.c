/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** look_cmd, look in a cone depending on the player level
*/

#include "command_handler.h"

void look_command(zappy_t *zappy, client_t *client, char **args)
{
    (void) zappy;
    (void) args;
    add_to_buffer(&client->out_buffer, "[player, food,,,]\n");
}
