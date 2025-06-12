/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Set time units per seconds
*/

#include <stdio.h>
#include <stdlib.h>

#include "command_handler.h"
#include "logs.h"

static void send_to_all_gui(zappy_t *zappy, const char *msg)
{
    for (client_t *actual = zappy->clients; actual != NULL;
        actual = actual->next) {
        add_to_buffer(actual->out_buffer, msg);
    }
}

void sst_command(zappy_t *zappy, client_t *client, char **args)
{
    int tps;
    char response[20];

    (void) client;
    if (pointlen(args) != 1) {
        LOG_WARNING("[%i]: Wrong amount of arguments for command sst."
            " Got %i but required %i", client->fd, pointlen(args), 1);
    } else {
        tps = atoi(&args[1][1]);
        if (tps < 1) {
            LOG_WARNING("[%i]: Minimum tps is 1", client->fd);
            return;
        }
        if (tps > 200) {
            LOG_WARNING("[%i]: Maximum tps is 200", client->fd);
            return;
        }
        zappy->parser->freq = tps;
        zappy->durationTick = 1.0 / zappy->parser->freq;
        snprintf(response, 19, "sst %i\n", zappy->parser->freq);
        send_to_all_gui(zappy, response);
    }
}
