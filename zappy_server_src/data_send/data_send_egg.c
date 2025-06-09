/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** data_sending
*/
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include "../include/client.h"
#include "../include/command.h"

static void send_enw(egg_t *egg, client_t *send)
{
    char enw_data[13 + get_size(egg->id) + get_size(egg->x) +
        get_size(egg->y)];

    snprintf(enw_data, sizeof(enw_data), "enw #%d #-1 %d %d\n",
        egg->id, egg->x, egg->y);
    add_to_buffer(&send->out_buffer, enw_data);
}

void send_eggs_data(zappy_t *zappy, client_t *c)
{
    egg_t *current_egg = zappy->map->eggs;

    while (current_egg != NULL) {
        send_enw(current_egg, c);
        current_egg = current_egg->next;
    }
}
