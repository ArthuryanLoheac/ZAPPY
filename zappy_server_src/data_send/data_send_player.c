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
        c->stats.id, c->stats.x, c->stats.y, c->stats.orientation,
        c->stats.level, c->stats.team_name);
    add_to_buffer(&send->out_buffer, pnw_data);
}

static void send_pin(client_t *c, client_t *send)
{
    char pin_data[256];
    stats_t s = c->stats;

    snprintf(pin_data, sizeof(pin_data),
        "pin #%d %d %d %d %d %d %d %d %d %d\n",
        s.id, s.x, s.y, s.inventory.food, s.inventory.linemate,
        s.inventory.deraumere, s.inventory.sibur, s.inventory.mendiane,
        s.inventory.phiras, s.inventory.thystame);
    add_to_buffer(&send->out_buffer, pin_data);
}

static void send_plv(client_t *c, client_t *send)
{
    char plv_data[256];

    snprintf(plv_data, sizeof(plv_data), "plv #%d %d\n",
        c->stats.id, c->stats.level);
    add_to_buffer(&send->out_buffer, plv_data);
}

void send_players_data(zappy_t *zappy, client_t *c)
{
    client_t *current_player = zappy->clients;

    while (current_player != NULL) {
        if (!current_player->is_graphic && current_player->is_connected && !current_player->is_waiting_id) {
            send_pnw(current_player, c);
            send_pin(current_player, c);
            send_plv(current_player, c);
        }
        current_player = current_player->next;
    }
}
