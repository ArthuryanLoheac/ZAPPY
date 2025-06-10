/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** This function is an equivalent of a map in C++
** You give it a string and you get the corresponding handler function
** (or NULL in case of error).
*/

#include <string.h>
#include <stdlib.h>

#include "command_handler.h"

static char *sanitize_name(const char* str)
{
    char *sanitized = malloc((strlen(str) + 1) * sizeof(char));

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z')
            sanitized[i] = str[i] - ('A');
        else
            sanitized[i] = str[i];
    }
    sanitized[strlen(str)] = '\0';
    return sanitized;
}

#include <stdio.h>

command_handler_t get_command_handler(const char *name)
{
    char *sanitized_name = sanitize_name(name);

    printf("Sanitized: '%s' => '%s'\n", name, sanitized_name);
    for (int i = 0; command_association_list[i].name != NULL; i++) {
        if (strcmp(sanitized_name, command_association_list[i].name) == 0)
            return command_association_list[i].func;
    }
    free(sanitized_name);
    return NULL;
}
