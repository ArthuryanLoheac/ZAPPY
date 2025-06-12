/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** bloc content
*/

#include <stdlib.h>
#include <stdio.h>

#include "command_handler.h"
#include "pointlen.h"
#include "logs.h"

void bct_command(zappy_t *zappy, client_t *client, char **args)
{
    if (pointlen(args) != 2) {
        add_to_buffer(&client->out_buffer, "ko\n");
        LOG_WARNING("[%i]: Wrong amount of arguments for command pin."
            " Got %i but required %i", client->fd, pointlen(args), 2);
    } else {
        send_bloc_content(atoi(args[1]), atoi(args[2]),
            zappy, client);
    }
}
