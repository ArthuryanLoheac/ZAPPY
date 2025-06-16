/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** take_command
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "command_handler.h"
#include "command.h"

static int calcul_dx(int dx, int width)
{
    if (dx > width / 2)
        dx -= width;
    if (dx < (-width) / 2)
        dx += width;
    if (dx > 1)
        dx = 1;
    if (dx < -1)
        dx = -1;
    return (dx);
}

static int calcul_dy(int dy, int height)
{
    if (dy > height / 2)
        dy -= height;
    if (dy < (-height) / 2)
        dy += height;
    if (dy > 1)
        dy = 1;
    if (dy < -1)
        dy = -1;
    return (dy);
}

static int from_to(cell_t from, cell_t to, int width, int height)
{
    int dx = to.x - from.x;
    int dy = to.y - from.y;
    int direction_map[3][3] = {{2, 1, 8}, {3, 0, 7}, {4, 5, 6}};

    dx = calcul_dx(dx, width);
    dy = calcul_dy(dy, height);
    return direction_map[dy + 1][dx + 1];
}

static void send_broadcast(client_t *client, int dir, char *text)
{
    char buffer[256];

    sprintf(buffer, "message %d, %s\n", dir, text);
    add_to_buffer(&client->out_buffer, buffer);
}

void broadcast_command(zappy_t *zappy, client_t *client, char **args)
{
    char buffer[256];
    client_t *curr_client = zappy->clients;
    int direction;

    sprintf(buffer, "pbc #%d %s\n", client->stats.id, args[0]);
    send_data_to_graphics(zappy, buffer);
    while (curr_client != NULL) {
        direction = calibrating_direction(from_to(
            zappy->map->grid[client->stats.x][client->stats.y],
            zappy->map->grid[curr_client->stats.x][curr_client->stats.y],
            zappy->parser->width, zappy->parser->height),
            curr_client->stats.orientation);
        send_broadcast(curr_client, direction, args[0]);
        curr_client = curr_client->next;
    }
}
