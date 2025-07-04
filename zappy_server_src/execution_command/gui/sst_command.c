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
#include "pointlen.h"

static bool is_tps_correct(int tps, int client_fd)
{
    if (tps < 1) {
        LOG_WARNING("[%i]: Minimum tps is 1", client_fd);
        return false;
    }
    if (tps > 200) {
        LOG_WARNING("[%i]: Maximum tps is 200", client_fd);
        return false;
    }
    return true;
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
        tps = atoi(args[0]);
        if (!is_tps_correct(tps, client->fd))
            return;
        zappy->parser->freq = tps;
        zappy->durationTick = 1.0 / zappy->parser->freq;
        snprintf(response, 19, "sst %i\n", zappy->parser->freq);
        send_data_to_graphics(zappy, response);
    }
}
