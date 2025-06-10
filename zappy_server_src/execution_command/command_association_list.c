/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** List of all handler commands & their nmae
*/

#include "command_handler.h"

const command_func_t command_association_list[] = {
    {"LOOK", look_command},
    {"FORWARD", forward_command},
    {"RIGHT", right_command},
    {"LEFT", left_command},
    {NULL, NULL}
};
