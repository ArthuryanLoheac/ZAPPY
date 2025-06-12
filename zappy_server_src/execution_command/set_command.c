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

static bool put_object(zappy_t *zappy, client_t *client,
    cell_t *cell, char *object)
{
    cell_t comp_cell = *cell;

    if (strcmp("food", object) == 0 && client->stats.inventory.food > 0)
        cell->nbr_food += 1;
    if (strcmp("linemate", object) == 0 && client->stats.inventory.linemate > 0)
        cell->nbr_linemate += 1;
    if (strcmp("deraumere", object) == 0 && client->stats.inventory.deraumere > 0)
        cell->nbr_deraumere += 1;
    if (strcmp("sibur", object) == 0 && client->stats.inventory.sibur > 0)
        cell->nbr_sibur += 1;
    if (strcmp("mendiane", object) == 0 && client->stats.inventory.mendiane > 0)
        cell->nbr_mendiane += 1;
    if (strcmp("phiras", object) == 0 && client->stats.inventory.phiras > 0)
        cell->nbr_phiras += 1;
    if (strcmp("thystame", object) == 0 && client->stats.inventory.thystame > 0)
        cell->nbr_thystame += 1;
    update_cell(zappy, *cell);
    return (cmp_cell(*cell, comp_cell));
}

static void remove_object_inventory(client_t *client, char *object)
{
    if (strcmp("food", object) == 0)
        client->stats.inventory.food -= 1;
    if (strcmp("linemate", object) == 0)
        client->stats.inventory.linemate -= 1;
    if (strcmp("deraumere", object) == 0)
        client->stats.inventory.deraumere -= 1;
    if (strcmp("sibur", object) == 0)
        client->stats.inventory.sibur -= 1;
    if (strcmp("mendiane", object) == 0)
        client->stats.inventory.mendiane -= 1;
    if (strcmp("phiras", object) == 0)
        client->stats.inventory.phiras -= 1;
    if (strcmp("thystame", object) == 0)
        client->stats.inventory.thystame -= 1;
}

void set_command(zappy_t *zappy, client_t *client, char **args)
{
    if (!put_object(zappy, client,
        &zappy->map->grid[client->stats.y][client->stats.x], args[0])) {
        remove_object_inventory(client, args[0]);
        add_to_buffer(&client->out_buffer, "ok\n");
    } else {
        add_to_buffer(&client->out_buffer, "ko\n");
    }
}
