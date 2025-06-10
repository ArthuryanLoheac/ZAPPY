/*
** EPITECH PROJECT, 2025
** server.c
** File description:
** Functions to create and manage the server
*/

#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>

#include "include/server.h"
#include "include/parser.h"
#include "include/logs.h"

static void listen_socket(int server_fd)
{
    if (listen(server_fd, 10) == -1)
        display_error("Failed to listen on server socket.");
}

static void bind_socket(int server_fd, int port)
{
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_fd, (struct sockaddr *)&server_addr,
        sizeof(server_addr)) == -1)
        display_error("Failed to bind server socket to port.");
}

static void config_socket(int server_fd)
{
    int opt = 1;

    if (setsockopt(server_fd, SOL_SOCKET,
        SO_REUSEADDR, &opt, sizeof(opt)) == -1)
        display_error("Failed to set socket options");
}

static int create_socket(void)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1)
        display_error("Failed to create server socket");
    return server_fd;
}

static struct pollfd *create_poll(int server_fd)
{
    struct pollfd *fds = malloc(sizeof(struct pollfd));

    if (fds == NULL)
        display_error("Memory allocation failed for poll file descriptors");
    fds[0].fd = server_fd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    return fds;
}

server_t *create_server(int port)
{
    server_t *server = malloc(sizeof(server_t));

    LOG_DEBUG("Starting server");
    if (server == NULL)
        display_error("Memory allocation failed for server structure");
    server->port = port;
    server->server_fd = create_socket();
    server->nb_fds = 1;
    server->fds = create_poll(server->server_fd);
    config_socket(server->server_fd);
    bind_socket(server->server_fd, port);
    listen_socket(server->server_fd);
    LOG_INFO("Server running on %d", port);
    return server;
}

void destroy_server(server_t *server)
{
    if (close(server->server_fd) == -1)
        display_error("Failed to close server socket");
    free(server->fds);
    free(server);
}
