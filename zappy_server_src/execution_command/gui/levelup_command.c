/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** team name
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "logs.h"

#include "command_handler.h"

void levelup_command(zappy_t *zappy, client_t *client, char **args)
{
    client_t *c = zappy->clients;
    int id = atoi(args[0]);
    char response[256];
    char response2[256];

    if (id < 0) {
        LOG_WARNING("[%i]: Invalid player ID for levelup command: %i",
            client->fd, id);
        add_to_buffer(&client->out_buffer, "ko\n");
        return;
    }
    while (c != NULL) {
        if (c->stats.id == id) {
            if (c->stats.level < 8) {
                c->stats.level++;
                snprintf(response, sizeof(response), "plv #%i %i\n",
                    c->stats.id, c->stats.level);
                send_data_to_graphics(zappy, response);
                snprintf(response2, sizeof(response2), "Current level : %i\n",
                    c->stats.level);
                add_to_buffer(&c->out_buffer, response2);
            } else {
                add_to_buffer(&client->out_buffer, "ko\n");
            }
            return;
        }
        c = c->next;
    }
}
