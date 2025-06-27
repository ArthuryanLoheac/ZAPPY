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
        if (client->stats.x == x && client->stats.y == y)
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
    add_value_buffer(&str, player, " player");
    add_value_buffer(&str, egg, " egg");
    add_value_buffer(&str, cell->nbr_food, " food");
    add_value_buffer(&str, cell->nbr_linemate, " linemate");
    add_value_buffer(&str, cell->nbr_deraumere, " deraumere");
    add_value_buffer(&str, cell->nbr_sibur, " sibur");
    add_value_buffer(&str, cell->nbr_mendiane, " mendiane");
    add_value_buffer(&str, cell->nbr_phiras, " phiras");
    add_value_buffer(&str, cell->nbr_thystame, " thystame");
    add_to_buffer(buffer, str);
}

static void move_forward_x(stats_t *stats, int *x, int *y, zappy_t *zappy)
{
    if (stats->orientation == 3)
        *y = (*y + 1) % zappy->parser->height;
    if (stats->orientation == 2)
        *x = (*x + 1) % zappy->parser->width;
    if (stats->orientation == 1) {
        *y -= 1;
        if (*y < 0)
            *y = zappy->parser->height - 1;
    }
    if (stats->orientation == 4) {
        *x -= 1;
        if (*x < 0)
            *x = zappy->parser->width - 1;
    }
}

static void move_forward_side(client_t *client, int *xyi, zappy_t *zappy,
    char **buffer)
{
    int ori = (client->stats.orientation + 1);

    if (ori > 4)
        ori = 1;
    if (ori == 3)
        xyi[1] += xyi[2];
    if (ori == 2)
        xyi[0] += xyi[2];
    if (ori == 1)
        xyi[1] -= xyi[2];
    if (ori == 4)
        xyi[0] -= xyi[2];
    if (xyi[1] < 0)
        xyi[1] = zappy->parser->height - 1;
    if (xyi[0] < 0)
        xyi[0] = zappy->parser->width - 1;
    xyi[1] = xyi[1] % zappy->parser->height;
    xyi[0] = xyi[0] % zappy->parser->width;
    add_to_buffer_tile(buffer, zappy, xyi[0], xyi[1]);
}

static void init_buffer(char **buffer)
{
    for (int i = 0; i < 2; i++)
        (*buffer)[i] = '\0';
    add_to_buffer(buffer, "[");
}

static void fill_variable(look_variable_t *variables, client_t *client)
{
    variables->level = client->stats.level;
    variables->x = client->stats.x;
    variables->y = client->stats.y;
}

static void check_add_comma(char **buffer, int i, int j, int level)
{
    if (j == i && i == level)
        return;
    add_to_buffer(buffer, ",");
}

void look_command(zappy_t *zappy, client_t *client, char **args)
{
    look_variable_t variables;
    int xyi_cpy[3] = {client->stats.x, client->stats.y, 0};
    char *buffer = malloc(2 * sizeof(char));

    if (client == NULL || zappy == NULL || buffer == NULL || !args)
        return;
    fill_variable(&variables, client);
    init_buffer(&buffer);
    for (int i = 0; i <= variables.level; i++) {
        for (int j = -i; j <= i; j++) {
            xyi_cpy[0] = variables.x;
            xyi_cpy[1] = variables.y;
            xyi_cpy[2] = j;
            move_forward_side(client, xyi_cpy, zappy, &buffer);
            check_add_comma(&buffer, i, j, variables.level);
        }
        move_forward_x(&client->stats, &variables.x, &variables.y, zappy);
    }
    add_to_buffer(&buffer, "]\n");
    add_to_buffer(&client->out_buffer, buffer);
}
