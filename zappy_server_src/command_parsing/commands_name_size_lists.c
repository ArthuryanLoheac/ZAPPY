/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** This array represents all commands the server handles and their duration
*/

#include <stddef.h>

#include "command.h"

const command_name_size_t player_command_name_size_list[] = {
    {"FORWARD", 7},
    {"RIGHT", 7},
    {"LEFT", 7},
    {"LOOK", 7},
    {"INVENTORY", 1},
    {"BROADCAST", 7},
    {"CONNECT_NBR", 0},
    {"FORK", 42},
    {"EJECT", 7},
    {"TAKE", 7},
    {"SET", 7},
    {"INCANTATION", 300},
    {NULL, -1}
};

const command_name_size_t gui_command_name_size_list[] = {
    {"MSZ", 0},
    {"BCT", 0},
    {"MCT", 0},
    {"TNA", 0},
    {"SGT", 0},
    {"SST", 0}
};
