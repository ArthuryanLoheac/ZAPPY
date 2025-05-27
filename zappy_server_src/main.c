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

static zappy_t zappy = {0};

int main(int ac, char **av)
{
    zappy_t *zappy_ptr = &zappy;

    zappy_ptr->parser = parse_arguments(ac, av);
    zappy_ptr->server = create_server(zappy_ptr->parser->port);
    destroy_parser(zappy_ptr->parser);
    destroy_server(zappy_ptr->server);
    return 0;
}
