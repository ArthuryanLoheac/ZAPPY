/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** take_command
*/

#include <string.h>
#include <stdio.h>

#include "command_handler.h"

void connect_nbr_command(zappy_t *zappy, client_t *client, char **args)
{
    int connect_nbr = 0;
    egg_t *egg;
    char response[50];

    (void)args;
    if (client == NULL || zappy == NULL || zappy->map == NULL)
        return;
    egg = zappy->map->eggs;
    while (egg != NULL) {
        if (strcmp(egg->team_name, client->stats.team_name) == 0)
            connect_nbr++;
        egg = egg->next;
    }
    snprintf(response, sizeof(response), "%d\n", connect_nbr);
    add_to_buffer(&client->out_buffer, response);
}
