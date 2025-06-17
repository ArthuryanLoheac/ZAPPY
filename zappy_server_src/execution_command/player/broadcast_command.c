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

static int from_to(cell_t from, cell_t to)
{
    if (from.x == to.x && from.y == to.y)
        return 0;
    if (from.x == to.x && from.y < to.y)
        return 1;
    if (from.x < to.x && from.y < to.y)
        return 2;
    if (from.x < to.x && from.y == to.y)
        return 3;
    if (from.x < to.x && from.y > to.y)
        return 4;
    if (from.x == to.x && from.y > to.y)
        return 5;
    if (from.x > to.x && from.y > to.y)
        return 6;
    if (from.x > to.x && from.y == to.y)
        return 7;
    if (from.x > to.x && from.y < to.y)
        return 8;
    return -1;
}

static void send_broadcast(client_t *client, int dir, char *text)
{
    char buffer[2570];

    sprintf(buffer, "message %d, %s\n", dir, text);
    add_to_buffer(&client->out_buffer, buffer);
}

static int rotate(int dir, int orient)
{
    if (dir == 0)
        return dir;
    if (orient == 0)
        return (dir + 6) % 8;
    if (orient == 2)
        return (dir + 2) % 8;
    if (orient == 3)
        return (dir + 4) % 8;
    return dir;
}

static void compute_message(char *textBuffer,
    zappy_t *zappy, client_t *source, client_t *dest)
{
    int movedX = (zappy->parser->width / 2) - source->stats.x;
    int movedY = (zappy->parser->height / 2) - source->stats.y;
    int newDestX = (dest->stats.x + movedX) % zappy->parser->width;
    int newDestY = (dest->stats.y + movedY) % zappy->parser->height;
    int direction = 0;

    if (newDestX < 0)
        newDestX += zappy->parser->width;
    if (newDestY < 0)
        newDestY += zappy->parser->height;
    direction = from_to(
        zappy->map->grid[zappy->parser->height / 2][zappy->parser->width / 2],
        zappy->map->grid[newDestY][newDestX]);
    direction = rotate(direction, dest->stats.orientation);
    send_broadcast(dest, direction, textBuffer);
}

static broadcast_every_client(zappy_t *zappy, client_t *client,
    char *textBuffer)
{
    char buffer[2570];
    client_t *curr_client = zappy->clients;

    sprintf(buffer, "pbc #%d %s\n", client->stats.id, textBuffer);
    send_data_to_graphics(zappy, buffer);
    while (curr_client != NULL) {
        if (!curr_client->is_graphic)
            compute_message(textBuffer, zappy, client, curr_client);
        curr_client = curr_client->next;
    }
}

void broadcast_command(zappy_t *zappy, client_t *client, char **args)
{
    char textBuffer[2560];

    if (args[0] == NULL) {
        add_to_buffer(&client->out_buffer, "ko\n");
        return;
    }
    for (int i = 0; args[i] != NULL; i++) {
        if (i > 0)
            strcat(textBuffer, " ");
        if (strlen(textBuffer) + strlen(args[i]) >= 2560) {
            add_to_buffer(&client->out_buffer, "ko\n");
            return;
        }
        strcat(textBuffer, args[i]);
    }
    broadcast_every_client(zappy, client, textBuffer);
    textBuffer[0] = '\0';
}
