/*
** EPITECH PROJECT, 2025
** main.c
** File description:
** Main entry file for the Zappy server
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>


#include "include/zappy.h"
#include "include/parser.h"
#include "include/server.h"
#include "logs.h"

static bool running = true;

void handle_down_server(int sig)
{
    (void)sig;
    running = false;
    printf("Server shutting down\n");
}

static void setup_down_server(void)
{
    struct sigaction sig = {0};

    sig.sa_handler = &handle_down_server;
    sigaction(SIGINT, &sig, NULL);
}

int main(int ac, char **av)
{
    zappy_t *zappy_ptr = malloc(sizeof(zappy_t));

    srand(time(NULL));
    zappy_ptr->parser = parse_arguments(ac, av);
    zappy_ptr->parser_str = parse_str_arguments(ac, av);
    zappy_ptr->server = create_server(zappy_ptr->parser->port);
    zappy_ptr->clients = NULL;
    zappy_ptr->map = init_starting_map(zappy_ptr);
    zappy_ptr->idNextClient = 0;
    zappy_ptr->end_game = false;
    zappy_ptr->winning_team = NULL;
    setup_down_server();
    start_server(zappy_ptr, &running);
    down_server(zappy_ptr);
    return 0;
}
