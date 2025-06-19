/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** level cheat
*/

#include "command_handler.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int len_array_array(char **object)
{
    int i = 0;

    while (object[i] != NULL)
        i += 1;
    printf("i = %d\n", i);
    return (i);
}

static bool modif_level(zappy_t *zappy, int id, int level)
{
    client_t *curr_client = zappy->clients;

    if (level < 1 || level > 8)
        return (false);
    while (curr_client != NULL) {
        if (curr_client->stats.id == id) {
            curr_client->stats.level = level;
            return (true);
        }
        curr_client = curr_client->next;
    }
    return (false);
}

void level_command(zappy_t *zappy, char **args)
{
    char buffer[256];

    if (len_array_array(args) != 2 || atoi(args[1]) == 0)
        return;
    if (!modif_level(zappy, atoi(args[0]), atoi(args[1])))
        return;
    sprintf(buffer, "plv #%s %s\n", args[0], args[1]);
    send_data_to_graphics(zappy, buffer);
    check_win(zappy);
}
