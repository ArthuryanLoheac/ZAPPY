/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** get_command_duration
*/

#include <stddef.h>
#include <string.h>

#include "command.h"

int get_command_duration(const char *name)
{
    for (int i = 0; command_name_size_list[i].name != NULL; i++)
        if (strcmp(command_name_size_list[i].name, name) == 0)
            return command_name_size_list[i].duration;
    return -1;
}
