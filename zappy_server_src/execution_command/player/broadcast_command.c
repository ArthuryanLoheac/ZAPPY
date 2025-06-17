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

static int from_to(cell_t from, cell_t to, zappy_t *zappy)
{
    (void)zappy;
    printf("from_to: from (%d, %d) to (%d, %d)\n",
        from.x, from.y, to.x, to.y);
    if (from.x == to.x && from.y == to.y)
        return 0;
    if (from.x == to.x && from.y < to.y)
        return 1; // North
    if (from.x < to.x && from.y < to.y)
        return 2; // North-West
    if (from.x < to.x && from.y == to.y)
        return 3; // West
    if (from.x < to.x && from.y > to.y)
        return 4; // South-West
    if (from.x == to.x && from.y > to.y)
        return 5; // South
    if (from.x > to.x && from.y > to.y)
        return 6; // South-East
    if (from.x > to.x && from.y == to.y)
        return 7; // East
    if (from.x > to.x && from.y < to.y)
        return 8; // North-East
    return -1; // Invalid direction
}

static void send_broadcast(client_t *client, int dir, char *text)
{
    char buffer[2570];

    sprintf(buffer, "message %d, %s\n", dir, text);
    add_to_buffer(&client->out_buffer, buffer);
}

static int rotate (int dir, int orient)
{
    if (dir == 0)
        return dir;
    if (orient == 0)
        return (dir + 6) % 8; // West
    if (orient == 2)
        return (dir + 2) % 8; // East
    if (orient == 3)
        return (dir + 4) % 8; // South
    return dir; // North
}

void broadcast_command(zappy_t *zappy, client_t *client, char **args)
{
    char buffer[2570];
    client_t *curr_client = zappy->clients;
    int direction;
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
    sprintf(buffer, "pbc #%d %s\n", client->stats.id, textBuffer);
    send_data_to_graphics(zappy, buffer);
    while (curr_client != NULL) {
        if (curr_client->is_graphic) {
            curr_client = curr_client->next;
            continue;
        }
        direction = from_to(
            zappy->map->grid[client->stats.y][client->stats.x],
            zappy->map->grid[curr_client->stats.y][curr_client->stats.x], zappy);
        direction = rotate(direction, curr_client->stats.orientation);
        send_broadcast(curr_client, direction, textBuffer);
        curr_client = curr_client->next;
    }
    textBuffer[0] = '\0';
}
