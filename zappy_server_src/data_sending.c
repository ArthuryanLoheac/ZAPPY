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

static void send_msz(zappy_t *zappy, int fd)
{
    char msz_data[7 + strlen(zappy->parser_str->width)
        + strlen(zappy->parser_str->height)];

    snprintf(msz_data, sizeof(msz_data), "msz %s %s\n",
        zappy->parser_str->width,
        zappy->parser_str->height);
    write(fd, msz_data, strlen(msz_data));
}

static void send_sgt(zappy_t *zappy, int fd)
{
    char sgt_data[6 + strlen(zappy->parser_str->freq)];

    snprintf(sgt_data, sizeof(sgt_data), "sgt %s\n",
        zappy->parser_str->freq);
    write(fd, sgt_data, strlen(sgt_data));
}

static void send_bct(starting_map_t *map, int fd, int x, int y)
{
    char bct_data[21 + get_size(x) + get_size(y)];
    cell_t cell = map->grid[y][x];

    snprintf(bct_data, sizeof(bct_data), "bct %d %d %d %d %d %d %d %d %d\n",
        x, y, cell.nbr_food, cell.nbr_linemate,
                cell.nbr_deraumere, cell.nbr_sibur, cell.nbr_mendiane,
                cell.nbr_phiras, cell.nbr_thystame);
    write(fd, bct_data, strlen(bct_data));
}

static void send_tna(int fd, char *name)
{
    char tna_data[6 + strlen(name)];

    snprintf(tna_data, sizeof(tna_data), "tna %s\n", name);
    write(fd, tna_data, strlen(tna_data));
}

static int return_coordinate(cell_t cell, int *egg_encoutered,
    int nbr_egg, int coord)
{
    if (cell.nbr_egg > 0)
        *egg_encoutered += 1;
    if (*egg_encoutered == nbr_egg)
        return (coord);
    return (-1);
}

static int find_x_egg(zappy_t *zappy, int nbr_egg)
{
    cell_t cell;
    int egg_encoutered = 0;
    int loop = -1;

    for (int i = 0; i < zappy->parser->height && loop == -1; i++) {
        for (int j = 0; j < zappy->parser->width && loop == -1; j++) {
            cell = zappy->map->grid[i][j];
            loop = return_coordinate(cell, &egg_encoutered,
                nbr_egg, i);
        }
    }
    return (loop);
}

static int find_y_egg(zappy_t *zappy, int nbr_egg)
{
    cell_t cell;
    int egg_encoutered = 0;
    int loop = -1;

    for (int i = 0; i < zappy->parser->height && loop == -1; i++) {
        for (int j = 0; j < zappy->parser->width && loop == -1; j++) {
            cell = zappy->map->grid[i][j];
            loop = return_coordinate(cell, &egg_encoutered,
                nbr_egg, j);
        }
    }
    return (loop);
}

static void send_enw(zappy_t *zappy, int fd, int nbr_egg)
{
    int x = find_x_egg(zappy, nbr_egg);
    int y = find_y_egg(zappy, nbr_egg);
    char enw_data[13 + get_size(nbr_egg) + get_size(x) + get_size(y)];

    snprintf(enw_data, sizeof(enw_data), "enw #%d #-1 %d %d\n",
        nbr_egg - 1, x, y);
    write(fd, enw_data, strlen(enw_data));
}

void send_data(zappy_t *zappy, int fd)
{
    int nbr_egg = 1;

    send_msz(zappy, fd);
    send_sgt(zappy, fd);
    for (int i = 0; i < zappy->parser->width; i++) {
        for (int j = 0; j < zappy->parser->height; j++) {
            send_bct(zappy->map, fd, i, j);
        }
    }
    for (int i = 0; zappy->parser->team_names[i] != NULL; i++) {
        send_tna(fd, zappy->parser->team_names[i]);
        for (int j = 0; j < 3; j++) {
            send_enw(zappy, fd, nbr_egg);
            nbr_egg += 1;
        }
    }
}
