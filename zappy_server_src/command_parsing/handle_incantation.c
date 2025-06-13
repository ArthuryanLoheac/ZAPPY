/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** process_command
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "client.h"
#include "logs.h"

static bool add_command_first(int duration, char **args, client_t *client)
{
    waitingCommands_t *new_command = malloc(sizeof(waitingCommands_t));
    int count = 1;
    int i = 0;

    if (new_command == NULL) {
        LOG_ERROR("Malloc failed for add_command:new_command");
        return false;
    }
    while (args[count])
        count++;
    new_command->command = malloc(sizeof(char *) * count);
    while (args[i]){
        new_command->command[i] = strdup(args[i]);
        i++;
    }
    args[i] = NULL;
    new_command->ticksLeft = duration;
    new_command->next = client->waiting_commands;
    client->waiting_commands = new_command;
    return true;
}

static int number_players(int x, int y, zappy_t *zappy_ptr) {
    client_t *client_cur = zappy_ptr->clients;
    int count = 0;

    while (client_cur != NULL) {
        if (client_cur->stats.x == x && client_cur->stats.y == y)
            count++;
        client_cur = client_cur->next;
    }
    return count;
}

int check_incantation_valid(zappy_t *zappy_ptr, client_t *client, int level)
{
    cell_t cell = zappy_ptr->map->grid[client->stats.y][client->stats.x];
    unsigned int nbPlayer = number_players(client->stats.x, client->stats.y, zappy_ptr);
    level -= 1;

    if (level > 6)
        return 0;
    if (nbPlayer >= requires_elevation_list[level].nb_player &&
        cell.nbr_linemate >= requires_elevation_list[level].nb_linemate &&
        cell.nbr_deraumere >= requires_elevation_list[level].nb_deraumere &&
        cell.nbr_sibur >= requires_elevation_list[level].nb_sibur &&
        cell.nbr_mendiane >= requires_elevation_list[level].nb_mendiane &&
        cell.nbr_phiras >= requires_elevation_list[level].nb_phiras &&
        cell.nbr_thystame >= requires_elevation_list[level].nb_thystame) {
        return 1;
    }
    return 0;
}

void handle_incantation(char **args, zappy_t *zappy_ptr, client_t *client)
{
    char **command;

    (void) zappy_ptr;
    if (strcmp(args[0], "INCANTATION") != 0)
        return;
    if (!check_incantation_valid(zappy_ptr, client, client->stats.level)){
        add_to_buffer(&client->out_buffer, "ko\n");
        return;
    }
    command = malloc(sizeof(char *) * 2);
    command[0] = strdup("START_INCANTATION");
    command[1] = NULL;
    add_command_first(0, command, client);
}