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
    {"CONNECT_NBR", connect_nbr_command},
    {"FORK", fork_command},
    {"START_INCANTATION", start_incantation_command},
    {"INCANTATION", incantation_command},
    {"EJECT", eject_command},
    {"BROADCAST", broadcast_command},
    {"INVENTORY", inventory_command},
    {NULL, NULL}
};

const command_func_t gui_command_association_list[] = {
    {"MSZ", msz_command},
    {"BCT", bct_command},
    {"MCT", mct_command},
    {"TNA", tna_command},
    {"PPO", ppo_command},
    {"PLV", plv_command},
    {"PIN", pin_command},
    {"SGT", sgt_command},
    {"SST", sst_command},
    {"LEVELUP", levelup_command},
    {"D_FORWARD", d_forward_command},
    {"D_BACKWARD", d_backward_command},
    {"D_RIGHT", d_right_command},
    {"D_LEFT", d_left_command},
    {"D_EJECT", d_eject_command},
    {"D_FORK", d_fork_command},
    {"D_RESSOURCE", d_ressource_command},
    {NULL, NULL}
};
