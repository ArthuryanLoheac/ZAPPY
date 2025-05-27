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

static zappy_t zappy = {0};

void handle_down_server(int sig)
{
    (void)sig;
    // down_server(&zappy);
    printf("Server shutting down.\n");
    exit(0);
}

static void setup_down_server(void)
{
    struct sigaction sig = {0};

    sig.sa_handler = &handle_down_server;
    sigaction(SIGINT, &sig, NULL);
}

int main(int ac, char **av)
{
    zappy_t *zappy_ptr = &zappy;

    zappy_ptr->parser = parse_arguments(ac, av);
    setup_down_server();
    // return run_server(&zappy);
    return 0;
}
