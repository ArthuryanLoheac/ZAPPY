/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** team name
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "logs.h"

#include "command_handler.h"

client_t *get_client_from_args(zappy_t *zappy, char **args)
{
    client_t *c = zappy->clients;
    int id = -1;

    if (zappy == NULL || !args || args[0] == NULL || strlen(args[0]) < 2)
        return NULL;
    id = atoi(args[0] + 1);
    if (id < 0)
        return NULL;
    while (c != NULL) {
        if (c->stats.id == id)
            return c;
        c = c->next;
    }
    return NULL;
}
