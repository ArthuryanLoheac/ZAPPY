/*
** EPITECH PROJECT, 2025
** main.c
** File description:
** Main entry file for the Zappy server
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <poll.h>
#include <sys/socket.h>

#include "include/zappy.h"
#include "include/client.h"
#include "include/command.h"

static void check_for_new_client(zappy_t *zappy)
{
    server_t *server = zappy->server;
    int new_fd = 0;

    if (server->fds[0].revents & POLLIN) {
        new_fd = accept(server->fds[0].fd, NULL, NULL);
        if (new_fd == -1)
            display_error("Failed to accept new client connection");
        add_client(zappy, new_fd);
    }
}

static void check_for_client_command(zappy_t *zappy)
{
    server_t *server = zappy->server;

    for (int i = 1; i < server->nb_fds; i++) {
        if (server->fds[i].revents & POLLIN)
            handle_client_command(zappy, server->fds[i].fd);
        if (server->fds[i].revents & POLLOUT)
            send_client_command(zappy, server->fds[i].fd);
    }
}

void start_server(zappy_t *zappy)
{
    server_t *server = zappy->server;
    int ready = 0;

    while (1) {
        ready = poll(server->fds, server->nb_fds,
            zappy->parser->freq * 1000);
        if (ready == -1)
            display_error("Poll error occurred");
        check_for_new_client(zappy);
        check_for_client_command(zappy);
    }
}

void down_server(zappy_t *zappy)
{
    if (!zappy || !zappy->server || !zappy->parser || !zappy->map)
        return;
    free_starting_map(zappy->map, zappy->parser->height);
    destroy_parser(zappy->parser);
    destroy_parser_str(zappy->parser_str);
    destroy_server(zappy->server);
    destroy_clients(zappy->clients);
}
