/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** List of all handler commands & their nmae
*/

#include "command_handler.h"

const command_func_t player_command_association_list[] = {
    {"LOOK", look_command},
    {"FORWARD", forward_command},
    {"RIGHT", right_command},
    {"LEFT", left_command},
    {"TAKE", take_command},
    {"SET", set_command},
    {NULL, NULL}
};

const command_func_t gui_command_association_list[] = {
    {NULL, NULL}
};
