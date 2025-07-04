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

static bool try_add_second_command(int duration, client_t *client,
    waitingCommands_t *new_command)
{
    new_command->ticksLeft = duration;
    if (client->waiting_commands == NULL) {
        client->waiting_commands = new_command;
        new_command->next = NULL;
        return true;
    }
    new_command->next = client->waiting_commands->next;
    client->waiting_commands->next = new_command;
    return true;
}

static bool add_last(waitingCommands_t *new_command, client_t *client)
{
    waitingCommands_t *command_cur;

    command_cur = client->waiting_commands;
    new_command->next = NULL;
    if (command_cur == NULL)
        client->waiting_commands = new_command;
    else {
        while (command_cur->next != NULL)
            command_cur = command_cur->next;
        command_cur->next = new_command;
    }
    return true;
}

bool add_command(int duration, char **args, client_t *client)
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
    new_command->command[i] = NULL;
    new_command->ticksLeft = duration;
    return add_last(new_command, client);
}

bool add_command_second(int duration, char **args, client_t *client)
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
    new_command->command[i] = NULL;
    return try_add_second_command(duration, client, new_command);
}

int get_size_commands(waitingCommands_t *commands)
{
    int size = 0;

    while (commands) {
        size++;
        commands = commands->next;
    }
    return size;
}

static int get_command_duration(client_t *client, char **args)
{
    if (client->is_graphic)
        return get_gui_command_duration(args[0]);
    else
        return get_player_command_duration(args[0]);
    return -1;
}

static void send_unknown(client_t *client, char **args)
{
    if (args && strcmp(args[0], "PING") == 0) {
        if (client->is_graphic) {
            add_to_buffer(&client->out_buffer, "PING\n");
            return;
        }
    }
    LOG_WARNING("[%i]: Received unknown command: %s\n",
        client->fd, args[0]);
    if (client->is_graphic)
        add_to_buffer(&client->out_buffer, "suc\n");
    else
        add_to_buffer(&client->out_buffer, "ko\n");
}

void push_command_to_queue(char **args, client_t *client, zappy_t *zappy_ptr)
{
    int command_duration = 0;

    if (client == NULL || zappy_ptr == NULL || args == NULL || args[0] == NULL
        || get_size_commands(client->waiting_commands) >= 10)
        return;
    set_upper(args);
    command_duration = get_command_duration(client, args);
    if (command_duration != -1) {
        LOG_INFO("[%i]: Received %s", client->fd, args[0]);
        if (handle_incantation(args, zappy_ptr, client) == 1)
            return;
        if (!add_command(command_duration, args, client))
            add_to_buffer(&client->out_buffer, "ko\n");
    } else
        send_unknown(client, args);
}
