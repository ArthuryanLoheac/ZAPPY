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
#include "client.h"

#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4

#define FRONT 1
#define LEFT 3
#define BACK 5
#define RIGHT 7

/**
 * @brief Computes the new position of a player based on their orientation.
 * This function updates the xy coordinates based on the player's orientation.
 * To go forward.
 *
 * @param x Pointer to the x coordinate.
 * @param y Pointer to the y coordinate.
 * @param stats Pointer to the player's stats containing orientation.
 * @param zappy Pointer to the zappy server structure containing map dimensions
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
 * @brief Get the cardinal direction from a tile to another.
 * 
 * @warning This function only works if the two tiles are aside.
 * 
 * @param tile1 X Y position of the first tile.
 * @param tile1 X Y position of the second tile.
 * @param xSize Width of the map.
 * @param ySize Height of the map.
 */
unsigned int get_direction_between_two_tiles(unsigned int tile1[2],
    unsigned int tile2[2], unsigned int xSize, unsigned int ySize)
{
    if (tile1[0] != tile2[0]) {
        if (tile1[0] == 0 && tile2[0] == xSize - 1)
            return WEST;
        if (tile1[0] == xSize - 1 && tile2[0] == 0)
            return EAST;
        return (tile1[0] < tile2[0] ? EAST : WEST );
    } else {
        if (tile1[1] == 0 && tile2[1] == ySize - 1)
            return SOUTH;
        if (tile1[1] == ySize - 1 && tile2[1] == 0)
            return NORTH;
        return (tile1[1] < tile2[1] ? SOUTH : NORTH);
    }
}

/**
 * @brief Get the orientation from which the eject is comming from relative
 * to the player's own orientation.
 * 
 * @paragraph The relative_diretion math method looks this bad because the
 * direciton of the player is stored between 1-4 internally, thus the need
 * for the -1 before using the modulo.
 * 
 * @param zappy Pointer to the main zappy struct, used to get the map size.
 * @param client Pointer to the client being ejected.
 * @param x Destination x coordinate.
 * @param y Destination y coordinate.
 */
static unsigned int get_eject_orientation_origin
    (zappy_t *zappy, client_t *client, int x, int y)
{
    unsigned int from[2] = {client->stats.x, client->stats.y};
    unsigned int to[2] = {x, y}; 
    unsigned int absolute_direction = get_direction_between_two_tiles(
        from, to, zappy->parser->width, zappy->parser->height);
    unsigned int relative_direction = ((absolute_direction - 1)
        - (client->stats.orientation - 1) + 4) % 4;
    
    switch (relative_direction) {
        case NORTH - 1:
            return FRONT;
        case EAST - 1:
            return RIGHT;
        case SOUTH - 1:
            return BACK;
        case WEST - 1:
            return LEFT;
    }
    return FRONT;
}

/**
 * @brief Updates the position of a client that has been ejected.
 * Send to all gui clients the new position of the ejected client.
 * Inform the pushed client that he has been ejected.
 *
 * @param client Pointer to the client being ejected.
 * @param x New x coordinate of the ejected client.
 * @param y New y coordinate of the ejected client.
 * @param zappy Pointer to the zappy server structure.
 */
static void update_pos_client_ejected(client_t *client, int x, int y,
    zappy_t *zappy)
{
    char buffer[256];

    client->stats.x = x;
    client->stats.y = y;
    sprintf(buffer, "ppo #%d %d %d %d\n",
            client->stats.id, x, y, client->stats.orientation);
    send_data_to_graphics(zappy, buffer);
    sprintf(buffer, "eject: %d\n", get_eject_orientation_origin
        (zappy, client, x, y));
    add_to_buffer(&client->out_buffer, buffer);
}

/**
 * @brief Sends a PEX message to the graphics client.
 * This sends a message to graphics clients indicating that a player has eject.
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
 * This function checks if there is a player in the position
 * where the clientis trying to eject.
 *
 * @param zappy Pointer to the zappy server structure.
 * @param client Pointer to the client executing the eject command.
 * @param xCheck X coordinate to check.
 * @param yCheck Y coordinate to check.
 * @return true if there is a player in front, false otherwise.
 */
static bool is_player_at_pos(zappy_t *zappy, client_t *client, int xCheck,
    int yCheck)
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
 * @brief I mean, duh, you got it right ?
 * 
 * @param zappy Pointer to the zappy server structure.
 * @param x Horizontal tile position.
 * @param y Vertical tile position.
 */
void destroy_eggs_on_tile(zappy_t *zappy, int x, int y)
{
    egg_t *last = NULL;

    for (egg_t *actual = zappy->map->eggs; actual != NULL;
        actual = actual->next) {
        if (actual->x == x && actual->y == y) {
            delete_egg(last, actual, zappy);
        } else {
            last = actual;
        }
    }
}

/**
 * @brief This function handles the eject command,
 * The player pushes another player in front of him
 *
 * @param zappy Pointer to the zappy server structure.
 * @param client Pointer to the client executing the eject command.
 * @param args Arguments passed with the command (not used).
 */
void eject_command(zappy_t *zappy, client_t *client, char **args)
{
    stats_t *self = &client->stats;
    int xFinal = self->x;
    int yFinal = self->y;

    (void) args;
    compute_forward_pos(&xFinal, &yFinal, self, zappy);
    if (!is_player_at_pos(zappy, client, self->x, self->y)) {
        add_to_buffer(&client->out_buffer, "ko\n");
        return;
    }
    add_to_buffer(&client->out_buffer, "ok\n");
    send_pex_to_graphics(zappy, client);
    for (client_t *zappyClient = zappy->clients; zappyClient != NULL;
        zappyClient = zappyClient->next) {
        if (zappyClient->stats.x == self->x && zappyClient->stats.y == self->y
            && zappyClient->stats.id != self->id)
            update_pos_client_ejected(zappyClient, xFinal, yFinal, zappy);
    }
}
