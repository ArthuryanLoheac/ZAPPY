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

static char to_lower_case(char c)
{
    return c + ('a' - 'A');
}

static char *sanitize_name(const char* str)
{
    char *sanitized = malloc((strlen(str) + 1) * sizeof(char));

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z')
            sanitized[i] = str[i];
        if (str[i] >= 'A' && str[i] <= 'Z')
            sanitized[i] = to_lower_case(str[i]);
    }
    return sanitized;
}

command_handler_t get_command_handler(const char *name)
{
    char *sanitized_name = sanitize_name(name);

    for (int i = 0; command_association_list[i].name != NULL; i++) {
        if (strcmp(name, command_association_list[i].name) == 0)
            return command_association_list[i].func;
    }
    free(sanitized_name);
    return NULL;
}
