/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** process_command
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

static bool is_in_teams(char *get, char **teams)
{
    char **team_ptr = teams;

    if (get == NULL || teams == NULL)
        return false;
    while (*team_ptr != NULL) {
        if (strcmp(get, *team_ptr) == 0)
            return true;
        team_ptr++;
    }
    return false;
}

static void handleCommand(char **args, client_t *client, zappy_t *zappy_ptr)
{
    if (client == NULL || zappy_ptr == NULL)
        return;
    printf("Processing command: %s\n", args[0]);
}

void process_command(char **args, client_t *client, zappy_t *zappy_ptr)
{
    if (client == NULL || args == NULL || args[0] == NULL)
        return;
    if (client->is_waiting_id) {
        if (is_in_teams(args[0], zappy_ptr->parser->team_names)){
            client->is_waiting_id = false;
            client->team_name = strdup(args[0]);
            char buffer[256];
            sprintf(buffer, "%d\n%d %d\n", zappy_ptr->idNextClient,
                    zappy_ptr->parser->width, zappy_ptr->parser->height);
            client->out_buffer = strdup(buffer);
            return;
        }
        if (strcmp(args[0], "GRAPHIC") == 0){
            send_data(zappy_ptr, client->fd);
            return;
        }
        client->out_buffer = strdup("ko\n");
    } else {
        handleCommand(args, client, zappy_ptr);
    }
}
