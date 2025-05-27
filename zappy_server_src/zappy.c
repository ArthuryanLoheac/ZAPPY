/*
** EPITECH PROJECT, 2025
** main.c
** File description:
** Main entry file for the Zappy server
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "include/zappy.h"
#include "include/parser.h"
#include "include/server.h"

void start_server(zappy_t *zappy)
{
}

void down_server(zappy_t *zappy)
{
    destroy_parser(zappy->parser);
    destroy_server(zappy->server);
}