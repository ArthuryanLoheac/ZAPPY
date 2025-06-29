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

static void level_up_client(zappy_t *zappy, client_t *c, client_t *gui)
{
    char buf_gui[256];
    char buf_client[256];

    if (c->stats.level < 8) {
        c->stats.level++;
        snprintf(buf_gui, sizeof(buf_gui), "plv #%i %i\n",
            c->stats.id, c->stats.level);
        send_data_to_graphics(zappy, buf_gui);
        snprintf(buf_client, sizeof(buf_client), "Current level : %i\n",
            c->stats.level);
        add_to_buffer(&c->out_buffer, buf_client);
    } else {
        add_to_buffer(&gui->out_buffer, "ko\n");
    }
    if (c->stats.level == 8)
        check_win(zappy);
}

void levelup_command(zappy_t *zappy, client_t *client, char **args)
{
    client_t *c = zappy->clients;
    int id = -1;

    if (!args || args[0] == NULL || strlen(args[0]) < 2)
        return;
    id = atoi(args[0] + 1);
    if (id < 0) {
        add_to_buffer(&client->out_buffer, "ko\n");
        return;
    }
    while (c != NULL) {
        if (c->stats.id == id && !c->is_graphic && !c->is_waiting_id) {
            level_up_client(zappy, c, client);
            return;
        }
        c = c->next;
    }
}
