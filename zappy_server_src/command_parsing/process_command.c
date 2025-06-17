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

static bool check_enough_space(char *get, zappy_t *zappy)
{
    egg_t *egg = zappy->map->eggs;

    while (egg) {
        if (strcmp(get, egg->team_name) == 0)
            return true;
        egg = egg->next;
    }
    return false;
}

static bool is_in_teams(char *get, char **teams, zappy_t *zappy)
{
    char **team_ptr = teams;

    if (get == NULL || teams == NULL)
        return false;
    while (*team_ptr != NULL) {
        if (strcmp(get, *team_ptr) == 0)
            return check_enough_space(get, zappy);
        team_ptr++;
    }
    return false;
}

void send_data_to_graphics(zappy_t *zappy, char *data)
{
    client_t *current_client = zappy->clients;

    while (current_client != NULL) {
        if (current_client->is_graphic && current_client->stats.team_name != NULL)
            add_to_buffer(&current_client->out_buffer, data);
        current_client = current_client->next;
    }
}

static egg_t *return_egg(zappy_t *zappy, char *team_name)
{
    egg_t *current = zappy->map->eggs;

    while (current) {
        if (strcmp(current->team_name, team_name) == 0)
            return current;
        current = current->next;
    }
    return NULL;
}

static void send_datas_new_player(stats_t *client,
    zappy_t *zappy_ptr, int eggId)
{
    char buffer1[256];
    char buffer2[256 * 2];
    char buffer3[256 * 3];

    sprintf(buffer1, "\npnw #%d %d %d %d %d %s\n", client->id, client->x,
        client->y, client->orientation, client->level, client->team_name);
    sprintf(buffer2, "%spin #%d %d %d %d %d %d %d %d %d %d\n", buffer1,
        client->id, client->x, client->y, client->inventory.food,
        client->inventory.linemate, client->inventory.deraumere,
        client->inventory.sibur, client->inventory.mendiane,
        client->inventory.phiras, client->inventory.thystame);
    sprintf(buffer3, "%sebo #%d\n", buffer2, eggId);
    send_data_to_graphics(zappy_ptr, buffer3);
}

static void new_connection_player(char **args, client_t *client,
    zappy_t *zappy_ptr)
{
    egg_t *egg = return_egg(zappy_ptr, args[0]);
    int eggId;

    if (client == NULL || args == NULL || zappy_ptr == NULL || egg == NULL)
        return;
    eggId = egg->id;
    client->stats.level = 1;
    client->stats.x = egg->x;
    client->stats.y = egg->y;
    client->stats.orientation = (rand() % 4) + 1;
    client->stats.team_name = strdup(args[0]);
    client->waiting_commands = NULL;
    delete_egg_team_name(zappy_ptr, args[0]);
    client->stats.id = zappy_ptr->idNextClient;
    zappy_ptr->idNextClient++;
    send_datas_new_player(&client->stats, zappy_ptr, eggId);
}

static bool check_graphic(char **args, client_t *client, zappy_t *zappy_ptr)
{
    if (strcmp(args[0], "GRAPHIC") == 0) {
        client->is_graphic = true;
        client->is_connected = true;
        client->is_waiting_id = false;
        client->stats.team_name = strdup("GRAPHIC");
        send_data(zappy_ptr, client);
        return true;
    }
    return false;
}

void process_command(char **args, client_t *client, zappy_t *zappy_ptr)
{
    char buffer[256];

    if (client == NULL || args == NULL || args[0] == NULL)
        return;
    if (client->is_waiting_id) {
        if (is_in_teams(args[0], zappy_ptr->parser->team_names, zappy_ptr)){
            client->is_graphic = false;
            client->is_waiting_id = false;
            sprintf(buffer, "%d\n%d %d\n", zappy_ptr->idNextClient,
                    zappy_ptr->parser->width, zappy_ptr->parser->height);
            client->out_buffer = strdup(buffer);
            new_connection_player(args, client, zappy_ptr);
            return;
        }
        if (check_graphic(args, client, zappy_ptr))
            return;
        client->out_buffer = strdup("ko\n");
    } else
        push_command_to_queue(args, client, zappy_ptr);
}
