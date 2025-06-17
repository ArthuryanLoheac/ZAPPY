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

static char *sanitize_name(const char *str)
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

command_handler_t get_player_command_handler(const char *name)
{
    char *sanitized_name = sanitize_name(name);

    for (int i = 0; player_command_association_list[i].name != NULL; i++) {
        if (strcmp(sanitized_name, player_command_association_list[i].name)
            == 0)
            return player_command_association_list[i].func;
    }
    free(sanitized_name);
    return NULL;
}

command_handler_t get_gui_command_handler(const char *name)
{
    char *sanitized_name = sanitize_name(name);

    for (int i = 0; gui_command_association_list[i].name != NULL; i++) {
        if (strcmp(sanitized_name, gui_command_association_list[i].name)
            == 0)
            return gui_command_association_list[i].func;
    }
    free(sanitized_name);
    return NULL;
}
