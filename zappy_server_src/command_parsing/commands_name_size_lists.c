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
    {"START_INCANTATION", 0},
    {"MAXLEVEL", 0},
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

const required_elevation_t requires_elevation_list[] = {
    {1, 1, 0, 0, 0, 0, 0},
    {2, 1, 1, 1, 0, 0, 0},
    {2, 2, 0, 1, 0, 2, 0},
    {4, 1, 1, 2, 0, 1, 0},
    {4, 1, 2, 1, 3, 0, 0},
    {6, 1, 2, 3, 0, 1, 0},
    {6, 2, 2, 2, 2, 2, 1},
};
