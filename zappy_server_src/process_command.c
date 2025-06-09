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

static char *realloc_strcat(char *dest, const char *src)
{
    size_t dest_len = dest ? strlen(dest) : 0;
    size_t src_len = src ? strlen(src) : 0;
    char *new_str = realloc(dest, dest_len + src_len + 1);

    if (new_str == NULL) {
        free(dest);
        return NULL; // Allocation failed
    }
    if (src) {
        strcpy(new_str + dest_len, src);
    }
    return new_str;
}

void send_data_to_graphics(zappy_t *zappy, char *data)
{
    client_t *current_client = zappy->clients;

    while (current_client != NULL) {
        if (current_client->is_graphic) {
            current_client->out_buffer = current_client->out_buffer ?
                realloc_strcat(current_client->out_buffer, data) : strdup(data);
        }
        current_client = current_client->next;
    }
}

static void handleCommand(char **args, client_t *client, zappy_t *zappy_ptr)
{
    if (client == NULL || zappy_ptr == NULL)
        return;
    printf("Processing command: %s\n", args[0]);
}

cell_t *get_next_egg(zappy_t *zappy_ptr)
{
    for (int i = 0; i < zappy_ptr->parser->height; i++) {
        for (int j = 0; j < zappy_ptr->parser->width; j++) {
            if (zappy_ptr->map->grid[i][j].nbr_egg > 0) {
                return &zappy_ptr->map->grid[i][j];
            }
        }
    }
    return NULL;
}

static void newConnectionPlayer(char **args, client_t *client, zappy_t *zappy_ptr)
{
    cell_t *egg = get_next_egg(zappy_ptr);
    char buffer1[1000];
    char buffer2[2000];

    if (client == NULL || args == NULL || zappy_ptr == NULL || egg == NULL)
        return;
    client->level = 1;
    client->x = egg->x;
    client->y = egg->y;
    client->orientation = (rand() % 4) + 1;
    client->team_name = strdup(args[0]);
    egg->nbr_egg--;
    client->id = zappy_ptr->idNextClient++;

    printf("New player connected: %s (ID: %d)\n", client->team_name, client->id);

    sprintf(buffer1, "pnw #%d %d %d %d %d %s\n", client->id, client->x,
        client->y, client->orientation, client->level, client->team_name);
    sprintf(buffer2, "%spin #%d %d %d %d %d %d %d %d %d %d\n", buffer1,
        client->id, client->x, client->y, client->nbr_food,
        client->nbr_linemate, client->nbr_deraumere, client->nbr_sibur,
        client->nbr_mendiane, client->nbr_phiras, client->nbr_thystame);
    //sprintf(buffer, "ebo #e\n");
    send_data_to_graphics(zappy_ptr, buffer2);
}

void process_command(char **args, client_t *client, zappy_t *zappy_ptr)
{
    if (client == NULL || args == NULL || args[0] == NULL)
        return;
    if (client->is_waiting_id) {
        if (is_in_teams(args[0], zappy_ptr->parser->team_names)){
            client->is_graphic = false;
            client->is_waiting_id = false;
            char buffer[256];
            sprintf(buffer, "%d\n%d %d\n", zappy_ptr->idNextClient,
                    zappy_ptr->parser->width, zappy_ptr->parser->height);
            client->out_buffer = strdup(buffer);
            newConnectionPlayer(args, client, zappy_ptr);
            return;
        }
        if (strcmp(args[0], "GRAPHIC") == 0) {
            client->is_graphic = true;
            send_data(zappy_ptr, client->fd);
            return;
        }
        client->out_buffer = strdup("ko\n");
    } else {
        handleCommand(args, client, zappy_ptr);
    }
}
