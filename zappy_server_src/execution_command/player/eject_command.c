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


/**
 * @brief Computes the new position of a player based on their orientation.
 * This function updates the x and y coordinates based on the player's orientation.
 * To go forward.
 *
 * @param x Pointer to the x coordinate.
 * @param y Pointer to the y coordinate.
 * @param stats Pointer to the player's stats containing orientation.
 * @param zappy Pointer to the zappy server structure containing map dimensions.
 */
static void compute_forward_pos(int *x, int *y, stats_t *stats, zappy_t *zappy)
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

/**
 * @brief Updates the position of a client that has been ejected.
 * Send to all gui clients the new position of the ejected client.
 *
 * @param client Pointer to the client being ejected.
 * @param x New x coordinate of the ejected client.
 * @param y New y coordinate of the ejected client.
 * @param zappy Pointer to the zappy server structure.
 */
static void update_pos_client_ejected(client_t *client, int x, int y, zappy_t *zappy)
{
    char buffer[256];

    client->stats.x = x;
    client->stats.y = y;
    sprintf(buffer, "ppo #%d %d %d %d\n",
            client->stats.id, x, y, client->stats.orientation);
    send_data_to_graphics(zappy, buffer);
}

/**
 * @brief Sends a PEX message to the graphics client.
 * This function sends a message to the graphics client indicating that a player has eject.
 *
 * @param zappy Pointer to the zappy server structure.
 * @param client Pointer to the client that has been ejected.
 */
static void send_pex_to_graphics(zappy_t *zappy, client_t *client)
{
    char buffer[256];

    sprintf(buffer, "pex #%d\n", client->stats.id);
    send_data_to_graphics(zappy, buffer);
}

/**
 * @brief Checks if a player is in front of the client.
 * This function checks if there is a player in the position where the client is trying to eject.
 *
 * @param zappy Pointer to the zappy server structure.
 * @param client Pointer to the client executing the eject command.
 * @param xCheck X coordinate to check.
 * @param yCheck Y coordinate to check.
 * @return true if there is a player in front, false otherwise.
 */
static bool is_player_at_pos(zappy_t *zappy, client_t *client, int xCheck, int yCheck)
{
    client_t *zappyClient = zappy->clients;

    while (zappyClient) {
        if (zappyClient->stats.x == xCheck && zappyClient->stats.y == yCheck
            && zappyClient->stats.id != client->stats.id)
            return true;
        zappyClient = zappyClient->next;
    }
    return false;
}

/**
 * @brief Eject command for a client.
 * This function handles the eject command, the player pushes another player in front of him
 *
 * @param zappy Pointer to the zappy server structure.
 * @param client Pointer to the client executing the eject command.
 * @param args Arguments passed with the command (not used).
 */
void eject_command(zappy_t *zappy, client_t *client, char **args)
{
    stats_t *self = &client->stats;
    int xFinal;
    int yFinal;
    client_t *zappyClient = zappy->clients;

    (void) args;
    xFinal = self->x;
    yFinal = self->y;
    compute_forward_pos(&xFinal, &yFinal, self, zappy);
    if (!is_player_at_pos(zappy, client, self->x, self->y)) {
        add_to_buffer(&client->out_buffer, "ko\n");
        return;
    }
    add_to_buffer(&client->out_buffer, "ok\n");
    send_pex_to_graphics(zappy, client);
    while (zappyClient) {
        if (zappyClient->stats.x == self->x && zappyClient->stats.y == self->y &&
            zappyClient->stats.id != self->id)
            update_pos_client_ejected(zappyClient, xFinal, yFinal, zappy);
        zappyClient = zappyClient->next;
    }
}
