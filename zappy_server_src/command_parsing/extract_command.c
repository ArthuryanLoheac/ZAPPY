/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** handle_commands
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"
#include "command.h"

static char *safe_strdup(const char *s)
{
    if (s == NULL) {
        fprintf(stderr, "safe_strdup: NULL pointer\n");
        display_error("Memory allocation failed");
    }
    return strdup(s);
}

static int count_tokens(char *str)
{
    char *temp = strdup(str);
    char *temp_ptr = temp;
    char *token = NULL;
    int count = 1;

    if (!temp)
        return -1;
    token = strtok(temp, " ");
    while (token != NULL) {
        count++;
        token = strtok(NULL, " ");
    }
    free(temp_ptr);
    return count;
}

static char **allocate_args_array(int count)
{
    return malloc(sizeof(char *) * (count));
}

static int free_args_array(char **args, char *temp_ptr, int i)
{
    for (int j = 0; j < i; j++) {
        if (args[j] == NULL)
            continue;
        free(args[j]);
        args[j] = NULL;
    }
    if (args)
        free(args);
    args = NULL;
    if (temp_ptr)
        free(temp_ptr);
    temp_ptr = NULL;
    return -1;
}

static int fill_args_array(char **args, char *command, int count)
{
    char *temp = strdup(command);
    char *temp_ptr = temp;
    char *token = NULL;
    int i = 0;

    if (!temp || !args)
        return -1;
    token = strtok(temp, " ");
    while (token != NULL && i < count) {
        args[i] = safe_strdup(token);
        if (!args[i])
            return free_args_array(args, temp_ptr, i);
        i++;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
    return 0;
}

static char **parse_command(char *command)
{
    char **args = NULL;
    int count = count_tokens(command);
    int result;

    if (count < 0)
        return NULL;
    args = allocate_args_array(count);
    if (!args)
        return NULL;
    result = fill_args_array(args, command, count);
    if (result < 0) {
        for (int i = 0; args[i] != NULL; i++)
            free(args[i]);
        free(args);
        return NULL;
    }
    return args;
}

void free_command_args(char **args)
{
    int i = 0;

    if (!args)
        return;
    while (args[i] != NULL) {
        if (args[i] == NULL)
            break;
        free(args[i]);
        args[i] = NULL;
        i++;
    }
    free(args);
    args = NULL;
}

static void process_command_line(client_t *client, char *command_line,
    zappy_t *zappy_ptr)
{
    char **args = parse_command(command_line);

    for (int i = 0; args && args[i] != NULL; i++)
    if (!args || !args[0])
        return;
    process_command(args, client, zappy_ptr);
    free_command_args(args);
}

void extract_commands(client_t *client, zappy_t *zappy_ptr)
{
    char *newline_pos = NULL;
    char *command_line = NULL;

    if (!client->in_buffer)
        return;
    newline_pos = strchr(client->in_buffer, '\n');
    while (newline_pos != NULL) {
        *newline_pos = '\0';
        command_line = strdup(client->in_buffer);
        if (command_line) {
            process_command_line(client, command_line, zappy_ptr);
            free(command_line);
        }
        memmove(client->in_buffer, newline_pos + 1,
                strlen(newline_pos + 1) + 1);
        newline_pos = strchr(client->in_buffer, '\n');
    }
}
