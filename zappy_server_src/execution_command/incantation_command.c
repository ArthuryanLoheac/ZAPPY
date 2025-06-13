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
 * @brief Send start elevation message to a client and schedule the command.
 * @param buffer Buffer to store the command message.
 * @param c Pointer to the client.
 */
static void send_elevation(char *buffer, client_t *c)
{
    char **command;
    char *buffercpy;

    command = malloc(sizeof(char *) * 2);
    command[0] = strdup("INCANTATION");
    command[1] = NULL;
    buffercpy = strdup(buffer);
    sprintf(buffer, "%s #%d", buffercpy, c->stats.id);
    add_to_buffer(&c->out_buffer, "Elevation underway\n");
    add_command_second(300, command, c);
    free(buffercpy);
}

/**
 * @brief Inform all clients on the same tile about the start of an incantation
 * @param zappy Pointer to the zappy server structure.
 * @param client Pointer to the client initiating the incantation.
 */
static void inform_all_clients(zappy_t *zappy, client_t *client)
{
    client_t *c = zappy->clients;
    char buffer[256];
    char *buffercpy;

    sprintf(buffer, "pic %d %d %d", client->stats.x, client->stats.y,
        client->stats.level);
    while (c) {
        if (c->stats.x == client->stats.x && c->stats.y == client->stats.y)
            send_elevation(buffer, c);
        c = c->next;
    }
    buffercpy = strdup(buffer);
    sprintf(buffer, "%s\n", buffercpy);
    send_data_to_graphics(zappy, buffer);
    free(buffercpy);
}

/**
 * @brief Start the incantation command for a client if valid.
 * @param zappy Pointer to the zappy server structure.
 * @param client Pointer to the client initiating the incantation.
 * @param args Command arguments (unused).
 */
void start_incantation_command(zappy_t *zappy, client_t *client, char **args)
{
    (void) args;
    if (!check_incantation_valid(zappy, client, client->stats.level))
        add_to_buffer(&client->out_buffer, "ko\n");
    else {
        inform_all_clients(zappy, client);
    }
}

/**
 * @brief Add a position to the list of successful elevations.
 * @param zappy Pointer to the zappy server structure.
 * @param x X-coordinate of the position.
 * @param y Y-coordinate of the position.
 * @param level Level of the elevation.
 */
static void add_pos_elevation(zappy_t *zappy, int x, int y, int level)
{
    pos_elevation_t *poses = zappy->pos_elevations;
    pos_elevation_t *new_pos;

    while (poses) {
        if (poses->x == x && poses->y == y)
            return;
        poses = poses->next;
    }
    new_pos = malloc(sizeof(pos_elevation_t));
    new_pos->x = x;
    new_pos->y = y;
    new_pos->level = level;
    new_pos->next = zappy->pos_elevations;
    zappy->pos_elevations = new_pos;
}

/**
 * @brief Add a position to the list of failed elevations.
 * @param zappy Pointer to the zappy server structure.
 * @param x X-coordinate of the position.
 * @param y Y-coordinate of the position.
 * @param level Level of the elevation.
 */
static void add_pos_elevation_failed(zappy_t *zappy, int x, int y, int level)
{
    pos_elevation_t *poses = zappy->pos_elevationsFail;
    pos_elevation_t *new_pos;

    while (poses) {
        if (poses->x == x && poses->y == y)
            return;
        poses = poses->next;
    }
    new_pos = malloc(sizeof(pos_elevation_t));
    new_pos->x = x;
    new_pos->y = y;
    new_pos->level = level;
    new_pos->next = zappy->pos_elevationsFail;
    zappy->pos_elevationsFail = new_pos;
}

/**
 * @brief Execute the incantation command for a client.
 * @param zappy Pointer to the zappy server structure.
 * @param client Pointer to the client executing the incantation.
 * @param args Command arguments (unused).
 */
void incantation_command(zappy_t *zappy, client_t *client, char **args)
{
    char buffer[256];
    char buffer2[256];

    (void) args;
    if (check_incantation_valid(zappy, client, client->stats.level)) {
        add_pos_elevation(zappy, client->stats.x, client->stats.y,
            client->stats.level);
        client->stats.level += 1;
        sprintf(buffer, "Current level: %d\n", client->stats.level);
        add_to_buffer(&client->out_buffer, buffer);
        sprintf(buffer2, "plv #%d %d\n", client->stats.id,
            client->stats.level);
        send_data_to_graphics(zappy, buffer2);
    } else {
        add_pos_elevation_failed(zappy, client->stats.x, client->stats.y,
            client->stats.level);
        add_to_buffer(&client->out_buffer, "ko\n");
    }
}
