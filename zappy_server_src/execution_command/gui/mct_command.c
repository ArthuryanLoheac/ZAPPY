/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** map content
*/

#include "command_handler.h"

void mct_command(zappy_t *zappy, client_t *client, char **args)
{
    (void) args;
    for (int y = 0; y < zappy->parser->height; y++) {
        for (int x = 0; x < zappy->parser->width; x++) {
            send_bloc_content(x, y, zappy, client);
        }
    }
}
