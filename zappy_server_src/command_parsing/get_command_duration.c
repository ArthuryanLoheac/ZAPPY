/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** get_command_duration
*/

#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "command.h"

int get_gui_command_duration(const char *name)
{
    for (int i = 0; gui_command_name_size_list[i].name != NULL; i++)
        if (strcmp(gui_command_name_size_list[i].name, name) == 0)
            return gui_command_name_size_list[i].duration;
    return -1;
}

int get_player_command_duration(const char *name)
{
    for (int i = 0; player_command_name_size_list[i].name != NULL; i++)
        if (strcmp(player_command_name_size_list[i].name, name) == 0)
            return player_command_name_size_list[i].duration;
    return -1;
}
