/*
** EPITECH PROJECT, 2025
** client.c
** File description:
** Functions to manage clients
*/

#include <poll.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "include/client.h"
#include "include/command.h"
#include "logs.h"

static void set_client_stat(stats_t *stat)
{
    stat->x = 0;
    stat->y = 0;
    stat->orientation = 1;
    stat->tickLife = 0;
    stat->inventory.food = 10;
    stat->inventory.linemate = 0;
    stat->inventory.deraumere = 0;
    stat->inventory.sibur = 0;
    stat->inventory.mendiane = 0;
    stat->inventory.phiras = 0;
    stat->inventory.thystame = 0;
}

static void add_client_data(zappy_t *zappy, int fd)
{
    client_t *client = malloc(sizeof(client_t));

    if (client == NULL)
        display_error("Memory allocation failed for client data");
    client->fd = fd;
    client->in_buffer = NULL;
    client->out_buffer = NULL;
    client->is_connected = true;
    client->is_waiting_id = true;
    client->waiting_commands = NULL;
    client->stats.team_name = NULL;
    client->stats.tickRemainingElevation = -1;
    client->stats.id = 0;
    client->stats.level = 1;
    set_client_stat(&client->stats);
    client->next = zappy->clients;
    zappy->clients = client;
    append_client_out_buffer(client, "WELCOME\n");
}

void add_client(zappy_t *zappy, int fd)
{
    server_t *server = zappy->server;

    if (server->nb_fds >= 1000) {
        LOG_WARNING("Connection attempt from client failed:"
            " maximum number of clients reached");
        if (close(fd) == -1)
            display_error("Failed to close client socket");
        return;
    }
    server->fds = realloc(server->fds, sizeof(struct pollfd) *
        (server->nb_fds + 1));
    if (server->fds == NULL)
        display_error("Memory allocation failed for poll file descriptors");
    server->fds[server->nb_fds].fd = fd;
    server->fds[server->nb_fds].events = POLLIN | POLLOUT;
    server->fds[server->nb_fds].revents = 0;
    add_client_data(zappy, fd);
    server->nb_fds++;
    LOG_DEBUG("Added new client(%i) to the server", fd);
}

static void remove_client_data(client_t **clients, int fd)
{
    client_t *current = *clients;
    client_t *prev = NULL;

    while (current != NULL && current->fd != fd) {
        prev = current;
        current = current->next;
    }
    if (current == NULL)
        return;
    if (prev == NULL)
        *clients = current->next;
    else
        prev->next = current->next;
    if (current->in_buffer != NULL)
        free(current->in_buffer);
    if (current->out_buffer != NULL)
        free(current->out_buffer);
    free(current);
}

void remove_client(zappy_t *zappy, int fd)
{
    server_t *server = zappy->server;
    int index = 0;

    while (server->fds[index].fd != fd) {
        index++;
        if (index == server->nb_fds)
            return;
    }
    for (int i = index; i < server->nb_fds - 1; i++)
        server->fds[i] = server->fds[i + 1];
    server->fds = realloc(server->fds, sizeof(struct pollfd) *
        (server->nb_fds - 1));
    if (server->fds == NULL)
        display_error("Memory allocation failed for poll file descriptors");
    remove_client_data(&zappy->clients, fd);
    server->nb_fds--;
}

void destroy_clients(client_t *clients)
{
    client_t *current = clients;
    client_t *next;

    while (current != NULL) {
        next = current->next;
        if (current->in_buffer != NULL)
            free(current->in_buffer);
        if (current->out_buffer != NULL)
            free(current->out_buffer);
        if (close(current->fd) == -1)
            display_error("Failed to close client socket");
        free(current);
        current = next;
    }
}
