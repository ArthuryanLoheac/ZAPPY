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

static void set_upper(char **args)
{
    int i = 0;

    if (!args || !args[0])
        return;
    while (args[0][i]) {
        if (args[0][i] >= 'a' && args[0][i] <= 'z')
            args[0][i] += 'A' - 'a';
        i++;
    }
}

static void add_command(int duration, char **args, client_t *client)
{
    waitingCommands_t *new_command = malloc(sizeof(waitingCommands_t));
    int count = 0;
    int i = 0;

    if (new_command == NULL)
        return;
    while (args[count])
        count++;
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
}

void exec_command(char **args, client_t *client, zappy_t *zappy_ptr)
{
    if (client == NULL || zappy_ptr == NULL)
        return;
    set_upper(args);
    if (strcmp(args[0], "FORWARD") == 0) {
        add_command(7, args, client);
    } else {
        printf("UNKNOWN command %s\n", args[0]);
        client->out_buffer = realloc_strcat(client->out_buffer, "ko\n");
    }
}