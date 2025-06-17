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

/**
 * @brief Computes the direction from one cell to another.
 * @param from The starting cell.
 * @param to The destination cell.
 * @return An integer representing the direction:
 * 0: same cell, 1: north, 2: northeast, 3: east, 4: southeast,
 * 5: south, 6: southwest, 7: west, 8: northwest.
 */
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

/**
 * @brief Sends a broadcast message to a specific client.
 * @param client The client to send the message to.
 * @param dir The direction of the broadcast.
 * @param text The message to send.
 */
static void send_broadcast(client_t *client, int dir, char *text)
{
    char buffer[2570];

    sprintf(buffer, "message %d, %s\n", dir, text);
    add_to_buffer(&client->out_buffer, buffer);
}

/**
 * @brief Rotates the direction based on the client's orientation.
 * @param dir The direction to rotate.
 * @param orient The orientation of the client.
 * @return The new direction after rotation.
 */
static int rotate(int dir, int orient)
{
    if (orient == 0)
        return (dir + 6) % 8;
    if (orient == 2)
        return (dir + 2) % 8;
    if (orient == 3)
        return (dir + 4) % 8;
    return dir;
}

/**
 * @brief Computes the message to be sent to a client based on their position
 * and orientation to be the shortest possible.
 * @param textBuffer The buffer containing the message to send.
 * @param zappy The zappy server instance.
 * @param source The client sending the broadcast.
 * @param dest The client receiving the broadcast.
 */
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

/**
 * @brief Broadcasts a message to every client in the game.
 * @param zappy The zappy server instance.
 * @param client The client sending the broadcast.
 * @param textBuffer The message to broadcast.
 */
static void broadcast_every_client(zappy_t *zappy, client_t *client,
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

/**
 * @brief Broadcasts a message to all clients in the game.
 * @param zappy The zappy server instance.
 * @param client The client sending the broadcast.
 * @param args The arguments for the broadcast command
 */
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
