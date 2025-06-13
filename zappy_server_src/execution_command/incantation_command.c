/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** take_command
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "command_handler.h"

static void inform_all_clients(zappy_t *zappy, client_t *client)
{
    client_t *c = zappy->clients;

    while (c) {
        if (c->stats.x == client->stats.x && c->stats.y == client->stats.y) {
            char **command = malloc(sizeof(char *) * 2);
            command[0] = strdup("INCANTATION");
            command[1] = NULL;

            add_to_buffer(&c->out_buffer, "Elevation underway\n");
            add_command_second(300, command, c);
        }
        c = c->next;
    }
}

void start_incantation_command(zappy_t *zappy, client_t *client, char **args)
{
    (void) args;
    if (!check_incantation_valid(zappy, client, client->stats.level))
        add_to_buffer(&client->out_buffer, "ko\n");
    else {
        inform_all_clients(zappy, client);
    }
}

void incantation_command(zappy_t *zappy, client_t *client, char **args)
{
    char buffer[256];
    char buffer2[256];

    (void) args;
    if (check_incantation_valid(zappy, client, client->stats.level)) {
        client->stats.level += 1;
        sprintf(buffer, "Current level: %d\n", client->stats.level);
        add_to_buffer(&client->out_buffer, buffer);
        sprintf(buffer2, "plv #%d %d\n", client->stats.id, client->stats.level);
        send_data_to_graphics(zappy, buffer2);
    } else
        add_to_buffer(&client->out_buffer, "ko\n");
}
