/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** process_command
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "client.h"
#include "logs.h"


static int number_players(int x, int y, zappy_t *zappy_ptr, int level) {
    client_t *client_cur = zappy_ptr->clients;
    int count = 0;

    while (client_cur != NULL) {
        if (client_cur->stats.x == x && client_cur->stats.y == y &&
            client_cur->stats.level == level)
            count++;
        client_cur = client_cur->next;
    }
    return count;
}

int check_incantation_valid(zappy_t *zappy_ptr, client_t *client, int level)
{
    cell_t cell = zappy_ptr->map->grid[client->stats.y][client->stats.x];
    unsigned int nbPlayer = number_players(client->stats.x, client->stats.y, zappy_ptr, level);
    level -= 1;

    if (level > 6)
        return 0;
    if (nbPlayer >= requires_elevation_list[level].nb_player &&
        cell.nbr_linemate >= requires_elevation_list[level].nb_linemate &&
        cell.nbr_deraumere >= requires_elevation_list[level].nb_deraumere &&
        cell.nbr_sibur >= requires_elevation_list[level].nb_sibur &&
        cell.nbr_mendiane >= requires_elevation_list[level].nb_mendiane &&
        cell.nbr_phiras >= requires_elevation_list[level].nb_phiras &&
        cell.nbr_thystame >= requires_elevation_list[level].nb_thystame) {
        return 1;
    }
    return 0;
}

void consume_incantation(zappy_t *zappy_ptr, int x, int y, int level)
{
    cell_t **grid = zappy_ptr->map->grid;
    char bct_data[256];
    level -= 1;

    if (level > 6)
        return;
    grid[y][x].nbr_linemate -= requires_elevation_list[level].nb_linemate;
    grid[y][x].nbr_deraumere -= requires_elevation_list[level].nb_deraumere;
    grid[y][x].nbr_sibur -= requires_elevation_list[level].nb_sibur;
    grid[y][x].nbr_mendiane -= requires_elevation_list[level].nb_mendiane;
    grid[y][x].nbr_phiras -= requires_elevation_list[level].nb_phiras;
    grid[y][x].nbr_thystame -= requires_elevation_list[level].nb_thystame;
    snprintf(bct_data, sizeof(bct_data), "bct %d %d %d %d %d %d %d %d %d\n",
        x, y, grid[y][x].nbr_food, grid[y][x].nbr_linemate,
        grid[y][x].nbr_deraumere, grid[y][x].nbr_sibur,
        grid[y][x].nbr_mendiane, grid[y][x].nbr_phiras,
        grid[y][x].nbr_thystame);
    send_data_to_graphics(zappy_ptr, bct_data);
}

int handle_incantation(char **args, zappy_t *zappy_ptr, client_t *client)
{
    char **command;

    (void) zappy_ptr;
    if (strcmp(args[0], "INCANTATION") != 0)
        return 0;
    command = malloc(sizeof(char *) * 2);
    command[0] = strdup("START");
    command[1] = NULL;
    printf("ADD START COMMAND\n");
    add_command(0, command, client);
    return 1;
}