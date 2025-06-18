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
#include <unistd.h>

#include "include/zappy.h"
#include "include/client.h"
#include "include/command.h"
#include "logs.h"

static void check_for_new_client(zappy_t *zappy)
{
    server_t *server = zappy->server;
    int new_fd = 0;

    if (server->fds[0].revents & POLLIN) {
        LOG_DEBUG("Connection attempt from a client");
        new_fd = accept(server->fds[0].fd, NULL, NULL);
        if (server->nb_fds >= 1000) {
            LOG_WARNING("Connection attempt from client failed:"
                " maximum number of clients reached");
            write(new_fd, "ko\n", 3);
            close(new_fd);
            return;
        }
        if (new_fd == -1) {
            LOG_WARNING("Connection attempt from client failed:"
                " accept syscall failed");
            return;
        }
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

/** @brief Print the server start message with configuration details.
 * @param zappy Pointer to the zappy_t structure containing server config.
 */
static void message_start_server(zappy_t *zappy)
{
    parser_str_t *parser_str = zappy->parser_str;

    printf("====== ZAPPY SERVER STARTED ======\n");
    printf("Port: %d\n", zappy->parser->port);
    printf("Map size: %s x %s\n", parser_str->width, parser_str->height);
    printf("Teams: ");
    for (int i = 0; i < zappy->parser->nb_teams; i++) {
        printf("%s", zappy->parser->team_names[i]);
        if (i < zappy->parser->nb_teams - 1)
            printf(", ");
    }
    printf("\nClients per team: %d\n", zappy->parser->clients_per_team);
    printf("Frequency: %s\n", parser_str->freq);
    LOG_FATAL("FATAL INFORMATION ENABLED");
    LOG_ERROR("ERROR INFORMATION ENABLED");
    LOG_WARNING("warning INFORMATION ENABLED");
    LOG_DEBUG("DEBUG INFORMATION ENABLED");
    LOG_INFO("INFO INFORMATION ENABLED");
    printf("===================================\n");
}

void start_server(zappy_t *zappy)
{
    server_t *server = zappy->server;
    int ready = 0;

    zappy->durationTick = 1.0 / zappy->parser->freq;
    zappy->durationTickLeft = zappy->durationTick;
    zappy->tickCount = 0;
    message_start_server(zappy);
    while (1) {
        check_ticks(zappy);
        ready = poll(server->fds, server->nb_fds,
            (1.0 / zappy->parser->freq) * 1000);
        if (ready == -1) {
            perror("Poll error occurred");
            continue;
        }
        check_for_new_client(zappy);
        check_for_client_command(zappy);
    }
}

void down_server(zappy_t *zappy)
{
    if (!zappy || !zappy->server || !zappy->parser || !zappy->map)
        return;
    free_starting_map(zappy->map);
    destroy_parser(zappy->parser);
    destroy_parser_str(zappy->parser_str);
    destroy_server(zappy->server);
    destroy_clients(zappy->clients);
}
