/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** team name
*/

#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "command_handler.h"

static void send_team_name(client_t *client)
{
    char response[6 + strlen(client->stats.team_name)];

    snprintf(response, 5 + strlen(client->stats.team_name),
        "tna %s\n", client->stats.team_name);
    add_to_buffer(&client->out_buffer, response);
}

void tna_command(zappy_t *zappy, client_t *client, char **args)
{
    (void) args;
    (void) client;
    for (client_t *actual = zappy->clients; actual != NULL;
        actual = actual->next) {
        send_team_name(client);
    }
}
