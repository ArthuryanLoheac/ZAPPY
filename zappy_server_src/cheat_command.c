/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** cheat_command
*/

#include "command_handler.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>

static int len_array_array(char **object)
{
    int i = 0;

    while (object[i] != NULL)
        i += 1;
    return (i);
}

static char **remove_name(char **command)
{
    char buffer[256];
    char **args;

    if (len_array_array(command) == 0)
        return (command);
    sprintf(buffer, "%s %s", command[1], command[2]);
    args = parse_command(buffer);
    return (args);
}

static cheat_handler_t get_cheat_handler(char *name)
{
    printf("%s\n", name);
    for (int i = 0; cheat_association_list[i].name != NULL; i++) {
        if (strcmp(name, cheat_association_list[i].name)
            == 0)
            return cheat_association_list[i].func;
    }
    return NULL;
}

void handle_cheat_function(zappy_t *zappy)
{
    cheat_handler_t handler;
    server_t *server = zappy->server;
    char buffer[256];
    char **command;

    if (server->fds[1].revents & POLLIN) {
        read(0, buffer, 256);
        command = parse_command(buffer);
        handler = get_cheat_handler(command[0]);
        if (handler == NULL)
            return;
        else
            handler(zappy, remove_name(command));
    }
}
