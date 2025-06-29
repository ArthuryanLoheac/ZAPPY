/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** take_command
*/

#include <string.h>
#include <stdio.h>
#include "command_handler.h"

static bool cmp_cell(cell_t one, cell_t two)
{
    if (one.nbr_food == two.nbr_food &&
        one.nbr_linemate == two.nbr_linemate &&
        one.nbr_deraumere == two.nbr_deraumere &&
        one.nbr_sibur == two.nbr_sibur &&
        one.nbr_mendiane == two.nbr_mendiane &&
        one.nbr_phiras == two.nbr_phiras &&
        one.nbr_thystame == two.nbr_thystame){
        return (true);
    }
    return (false);
}

static bool put_object(zappy_t *zappy, stats_t *stats,
    cell_t *cell, char *object)
{
    cell_t comp_cell = *cell;

    if (object == NULL)
        return (true);
    if (strcmp("food", object) == 0 && stats->inventory.food > 0)
        cell->nbr_food += 1;
    if (strcmp("linemate", object) == 0 && stats->inventory.linemate > 0)
        cell->nbr_linemate += 1;
    if (strcmp("deraumere", object) == 0 && stats->inventory.deraumere > 0)
        cell->nbr_deraumere += 1;
    if (strcmp("sibur", object) == 0 && stats->inventory.sibur > 0)
        cell->nbr_sibur += 1;
    if (strcmp("mendiane", object) == 0 && stats->inventory.mendiane > 0)
        cell->nbr_mendiane += 1;
    if (strcmp("phiras", object) == 0 && stats->inventory.phiras > 0)
        cell->nbr_phiras += 1;
    if (strcmp("thystame", object) == 0 && stats->inventory.thystame > 0)
        cell->nbr_thystame += 1;
    update_cell(zappy, *cell);
    return (cmp_cell(*cell, comp_cell));
}

static void remove_object_inventory(stats_t *stats, char *object)
{
    if (strcmp("food", object) == 0)
        stats->inventory.food -= 1;
    if (strcmp("linemate", object) == 0)
        stats->inventory.linemate -= 1;
    if (strcmp("deraumere", object) == 0)
        stats->inventory.deraumere -= 1;
    if (strcmp("sibur", object) == 0)
        stats->inventory.sibur -= 1;
    if (strcmp("mendiane", object) == 0)
        stats->inventory.mendiane -= 1;
    if (strcmp("phiras", object) == 0)
        stats->inventory.phiras -= 1;
    if (strcmp("thystame", object) == 0)
        stats->inventory.thystame -= 1;
}

static int get_ressource_id(const char *object)
{
    if (strcmp(object, "food") == 0)
        return 0;
    if (strcmp(object, "linemate") == 0)
        return 1;
    if (strcmp(object, "deraumere") == 0)
        return 2;
    if (strcmp(object, "sibur") == 0)
        return 3;
    if (strcmp(object, "mendiane") == 0)
        return 4;
    if (strcmp(object, "phiras") == 0)
        return 5;
    if (strcmp(object, "thystame") == 0)
        return 6;
    return -1;
}

static void send_all_data(zappy_t *zappy, char *buffer, char *buffer2)
{
    send_data_to_graphics(zappy, buffer);
    send_data_to_graphics(zappy, buffer2);
}

void set_command(zappy_t *zappy, client_t *client, char **args)
{
    char buffer[256];
    char buffer2[256];

    if (client == NULL || !zappy || !args || !args[0] || !zappy->map)
        return;
    if (!put_object(zappy, &client->stats,
        &zappy->map->grid[client->stats.y][client->stats.x], args[0])) {
        remove_object_inventory(&client->stats, args[0]);
        add_to_buffer(&client->out_buffer, "ok\n");
        sprintf(buffer, "pdr #%d %d\n", client->stats.id,
            get_ressource_id(args[0]));
        sprintf(buffer2, "pin #%d %d %d %d %d %d %d %d %d %d\n",
        client->stats.id, client->stats.x, client->stats.y,
        client->stats.inventory.food, client->stats.inventory.linemate,
        client->stats.inventory.deraumere, client->stats.inventory.sibur,
        client->stats.inventory.mendiane, client->stats.inventory.phiras,
        client->stats.inventory.thystame);
        send_all_data(zappy, buffer, buffer2);
    } else
        add_to_buffer(&client->out_buffer, "ko\n");
}
