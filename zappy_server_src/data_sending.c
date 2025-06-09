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

#include "include/client.h"
#include "include/command.h"

static int get_size(int nbr)
{
    if (nbr >= 1000)
        return (4);
    if (nbr >= 100)
        return (3);
    if (nbr >= 10)
        return (2);
    return (1);
}

static void add_to_buffer(char **buffer, const char *data)
{
    size_t old_size = *buffer ? strlen(*buffer) : 0;
    size_t new_size = old_size + strlen(data) + 1; // +1 for null terminator
    char *new_buffer = realloc(*buffer, new_size);

    if (new_buffer == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    *buffer = new_buffer;
    strcpy(*buffer + old_size, data);
}

static void send_msz(zappy_t *zappy, client_t *send)
{
    char msz_data[7 + strlen(zappy->parser_str->width)
        + strlen(zappy->parser_str->height)];

    snprintf(msz_data, sizeof(msz_data), "msz %s %s\n",
        zappy->parser_str->width,
        zappy->parser_str->height);
    add_to_buffer(&send->out_buffer, msz_data);
}

static void send_sgt(zappy_t *zappy, client_t *send)
{
    char sgt_data[6 + strlen(zappy->parser_str->freq)];

    snprintf(sgt_data, sizeof(sgt_data), "sgt %s\n",
        zappy->parser_str->freq);
    add_to_buffer(&send->out_buffer, sgt_data);
}

static void send_bct(starting_map_t *map, int x, int y, client_t *send)
{
    char bct_data[21 + get_size(x) + get_size(y)];
    cell_t cell = map->grid[y][x];

    snprintf(bct_data, sizeof(bct_data), "bct %d %d %d %d %d %d %d %d %d\n",
        x, y, cell.nbr_food, cell.nbr_linemate,
                cell.nbr_deraumere, cell.nbr_sibur, cell.nbr_mendiane,
                cell.nbr_phiras, cell.nbr_thystame);
    add_to_buffer(&send->out_buffer, bct_data);
}

static void send_tna(char *name, client_t *send)
{
    char tna_data[6 + strlen(name)];

    snprintf(tna_data, sizeof(tna_data), "tna %s\n", name);
    add_to_buffer(&send->out_buffer, tna_data);
}

static void send_enw(egg_t *egg, client_t *send)
{
    char enw_data[13 + get_size(egg->id) + get_size(egg->x) + get_size(egg->y)];

    snprintf(enw_data, sizeof(enw_data), "enw #%d #-1 %d %d\n",
        egg->id, egg->x, egg->y);
    add_to_buffer(&send->out_buffer, enw_data);
}

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

    snprintf(pin_data, sizeof(pin_data), "pin #%d %d %d %d %d %d %d %d %d %d\n",
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

void send_data(zappy_t *zappy, client_t *c)
{
    send_msz(zappy, c);
    send_sgt(zappy, c);
    for (int i = 0; i < zappy->parser->width; i++) {
        for (int j = 0; j < zappy->parser->height; j++) {
            send_bct(zappy->map, i, j, c);
        }
    }
    for (int i = 0; zappy->parser->team_names[i] != NULL; i++) {
        send_tna(zappy->parser->team_names[i], c);
    }
    egg_t *current_egg = zappy->map->eggs;
    while (current_egg != NULL) {
        send_enw(current_egg, c);
        current_egg = current_egg->next;
    }
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
