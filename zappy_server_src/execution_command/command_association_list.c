/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** List of all handler commands & their nmae
*/

#include "command_handler.h"

const command_func_t command_association_list[] = {
    {"look", look_command},
    {"forward", forward_command},
    {"right", right_command},
    {"left", left_command},
    {NULL, NULL}
};
