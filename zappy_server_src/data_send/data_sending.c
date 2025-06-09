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
    send_eggs_data(zappy, c);
    send_players_data(zappy, c);
}

void add_to_buffer(char **buffer, const char *data)
{
    size_t old_size = *buffer ? strlen(*buffer) : 0;
    size_t new_size = old_size + strlen(data) + 1;
    char *new_buffer = realloc(*buffer, new_size);

    if (new_buffer == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    *buffer = new_buffer;
    strcpy(*buffer + old_size, data);
}

int get_size(int nbr)
{
    if (nbr >= 1000)
        return (4);
    if (nbr >= 100)
        return (3);
    if (nbr >= 10)
        return (2);
    return (1);
}