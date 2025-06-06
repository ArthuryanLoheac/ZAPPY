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


#include "include/zappy.h"
#include "include/parser.h"
#include "include/server.h"

static zappy_t zappy = {0};

void handle_down_server(int sig)
{
    (void)sig;
    down_server(&zappy);
    printf("Server shutting down\n");
    exit(0);
}

static void setup_down_server(void)
{
    struct sigaction sig = {0};

    sig.sa_handler = &handle_down_server;
    sigaction(SIGINT, &sig, NULL);
}

static int check_args(zappy_t *zappy_ptr)
{
    char **teams = zappy_ptr->parser->team_names;

    while (*teams != NULL) {
        if (strcmp(*teams, "GRAPHIC") == 0) {
            printf("The team GRAPHIC is reserved for the graphical client.\n");
            return 1;
        }
        teams++;
    }
    return 0;
}

int main(int ac, char **av)
{
    zappy_t *zappy_ptr = &zappy;

    srand(time(NULL));
    zappy_ptr->parser = parse_arguments(ac, av);
    zappy_ptr->parser_str = parse_str_arguments(ac, av);
    if (check_args(zappy_ptr) != 0) {
        destroy_parser_str(zappy_ptr->parser_str);
        destroy_parser(zappy_ptr->parser);
        return 84;
    }
    zappy_ptr->server = create_server(zappy_ptr->parser->port);
    zappy_ptr->clients = NULL;
    zappy_ptr->map = init_starting_map(zappy_ptr, 2);
    setup_down_server();
    start_server(zappy_ptr);
    down_server(zappy_ptr);
    return 0;
}
