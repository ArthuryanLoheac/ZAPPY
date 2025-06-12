/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** team name
*/

#include <stddef.h>

#include "command_handler.h"

void tna_command(zappy_t *zappy, client_t *client, char **args)
{
    (void) args;
    (void) client;
    for (client_t *actual = zappy->clients; actual != NULL;
        actual = actual->next) {
        add_to_buffer(&actual->out_buffer, actual->stats.team_name);
    }
}
