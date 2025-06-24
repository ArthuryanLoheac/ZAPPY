/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** inventory_command
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "command_handler.h"
#include "command.h"

/**
 * @brief Send their inventory to the client
 * @param zappy The zappy server instance.
 * @param client The client sending the broadcast.
 * @param args The arguments for the broadcast command
 */
void inventory_command(zappy_t *zappy, client_t *client, char **args)
{
    char buffer[256];

    if (client == NULL)
        return;
    (void) args;
    (void) zappy;
    sprintf(buffer, "[food %d, linemate %d, deraumere %d, sibur %d, "
        "mendiane %d, phiras %d, thystame %d]\n",
        client->stats.inventory.food, client->stats.inventory.linemate,
        client->stats.inventory.deraumere, client->stats.inventory.sibur,
        client->stats.inventory.mendiane, client->stats.inventory.phiras,
        client->stats.inventory.thystame);
    add_to_buffer(&client->out_buffer, buffer);
}
