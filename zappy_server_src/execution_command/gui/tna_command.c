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

static void send_team_name(client_t *gui, const char *team_name)
{
    char response[6 + strlen(team_name)];

    snprintf(response, 5 + strlen(team_name),
        "tna %s\n", team_name);
    add_to_buffer(&gui->out_buffer, response);
}

void tna_command(zappy_t *zappy, client_t *client, char **args)
{
    (void) args;
    (void) client;
    for (int i = 0; zappy->parser->team_names[i]; i++) {
        send_team_name(client, zappy->parser->team_names[i]);
    }
}
