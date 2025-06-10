/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** look_cmd, look in a cone depending on the player level
*/

#include "command_handler.h"

#include <stdio.h>
#include <stdlib.h>

static int get_nb_egg_at_pos(zappy_t *zappy, int x, int y)
{
    egg_t *egg = zappy->map->eggs;
    int count = 0;

    while (egg != NULL) {
        if (egg->x == x && egg->y == y)
            count++;
        egg = egg->next;
    }
    return count;
}

static int get_nb_player_at_pos(zappy_t *zappy, int x, int y)
{
    client_t *client = zappy->clients;
    int count = 0;

    while (client != NULL) {
        if (client->x == x && client->y == y)
            count++;
        client = client->next;
    }
    return count;
}

static void add_value_buffer(char **buffer, int value, char *str)
{
    for (int i = 0; i < value; i++)
        add_to_buffer(buffer, str);
}

static void add_to_buffer_tile(char **buffer, zappy_t *zappy, int x, int y)
{
    cell_t *cell = &zappy->map->grid[y][x];
    int egg = get_nb_egg_at_pos(zappy, x, y);
    int player = get_nb_player_at_pos(zappy, x, y);
    char *str = malloc(256 * sizeof(char));

    for (int i = 0; i < 256; i++)
        str[i] = 0;
    add_value_buffer(&str, player, "player ");
    add_value_buffer(&str, egg, "egg ");
    add_value_buffer(&str, cell->nbr_food, "food ");
    add_value_buffer(&str, cell->nbr_linemate, "linemate ");
    add_value_buffer(&str, cell->nbr_deraumere, "deraumere ");
    add_value_buffer(&str, cell->nbr_sibur, "sibur ");
    add_value_buffer(&str, cell->nbr_mendiane, "mendiane ");
    add_value_buffer(&str, cell->nbr_phiras, "phiras ");
    add_value_buffer(&str, cell->nbr_thystame, "thystame ");
    add_to_buffer(&str, ",");
    add_to_buffer(buffer, str);
}

static void move_forward_x(client_t *client, int *x, int *y, zappy_t *zappy)
{
    if (client->orientation == 3)
        *y = (*y + 1) % zappy->parser->height;
    if (client->orientation == 2)
        *x = (*x + 1) % zappy->parser->width;
    if (client->orientation == 1) {
        *y -= 1;
        if (*y < 0)
            *y = zappy->parser->height - 1;
    }
    if (client->orientation == 4) {
        *x -= 1;
        if (*x < 0)
            *x = zappy->parser->width - 1;
    }
}

static void move_forward_side(client_t *client, int *xyi, zappy_t *zappy,
    char **buffer)
{
    int or = (client->orientation + 1);

    if (or > 4)
        or = 1;
    if (or == 3)
        xyi[1] += xyi[2];
    if (or == 2)
        xyi[0] += xyi[2];
    if (or == 1)
        xyi[1] -= xyi[2];
    if (or == 4)
        xyi[0] -= xyi[2];
    if (xyi[1] < 0)
        xyi[1] = zappy->parser->height - 1;
    if (xyi[0] < 0)
        xyi[0] = zappy->parser->width - 1;
    xyi[1] = xyi[1] % zappy->parser->height;
    xyi[0] = xyi[0] % zappy->parser->width;
    add_to_buffer_tile(buffer, zappy, xyi[0], xyi[1]);
}

void look_command(zappy_t *zappy, client_t *client, char **args)
{
    int level = client->level;
    int x = client->x;
    int y = client->y;
    int xyi_cpy[3] = {client->x, client->y, 0};
    char *buffer = malloc(2 * sizeof(char));

    (void) args;
    for (int i = 0; i < 2; i++)
        buffer[i] = '\0';
    add_to_buffer(&buffer, "[");
    for (int i = 0; i <= level; i++) {
        for (int j = -i; j <= i; j++) {
            xyi_cpy[0] = x;
            xyi_cpy[1] = y;
            xyi_cpy[2] = j;
            move_forward_side(client, xyi_cpy, zappy, &buffer);
        }
        move_forward_x(client, &x, &y, zappy);
    }
    add_to_buffer(&buffer, "]\n");
    add_to_buffer(&client->out_buffer, buffer);
}
