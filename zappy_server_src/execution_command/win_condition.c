/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** win_condition
*/

#include "command_handler.h"
#include <stdio.h>
#include <string.h>

static int check_client(zappy_t *zappy, int i)
{
    client_t *curr_client = zappy->clients;
    int level_8_counter = 0;

    while (curr_client != NULL) {
        if (curr_client->is_graphic) {
            curr_client = curr_client->next;
            continue;
        }
        if (curr_client->stats.team_name == zappy->parser->team_names[i] &&
            curr_client->stats.level == 8) {
            level_8_counter += 1;
        }
        curr_client = curr_client->next;
    }
    return (level_8_counter);
}

void check_win(zappy_t *zappy)
{
    char buffer[256];

    for (int i = 0; i < zappy->parser->nb_teams; i++) {
        if (check_client(zappy, i) >= 6) {
            zappy->end_game = true;
            zappy->winning_team = strdup(zappy->parser->team_names[i]);
            sprintf(buffer, "seg %s\n", zappy->winning_team);
            return;
        }
    }
}
