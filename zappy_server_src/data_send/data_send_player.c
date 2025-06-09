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

static void send_pnw(client_t *c, client_t *send)
{
    char pnw_data[256];

    snprintf(pnw_data, sizeof(pnw_data), "pnw #%d %d %d %d %d %s\n",
        c->id, c->x, c->y, c->orientation, c->level, c->team_name);
    add_to_buffer(&send->out_buffer, pnw_data);
}

static void send_pin(client_t *c, client_t *send)
{
    char pin_data[256];

    snprintf(pin_data, sizeof(pin_data),
        "pin #%d %d %d %d %d %d %d %d %d %d\n",
        c->id, c->x, c->y, c->nbr_food, c->nbr_linemate,
        c->nbr_deraumere, c->nbr_sibur, c->nbr_mendiane,
        c->nbr_phiras, c->nbr_thystame);
    add_to_buffer(&send->out_buffer, pin_data);
}

static void send_plv(client_t *c, client_t *send)
{
    char plv_data[256];

    snprintf(plv_data, sizeof(plv_data), "plv #%d %d\n",
        c->id, c->level);
    add_to_buffer(&send->out_buffer, plv_data);
}

void send_players_data(zappy_t *zappy, client_t *c)
{
    client_t *current_player = zappy->clients;

    while (current_player != NULL) {
        if (!current_player->is_graphic) {
            send_pnw(current_player, c);
            send_pin(current_player, c);
            send_plv(current_player, c);
        }
        current_player = current_player->next;
    }
}
