/*
** EPITECH PROJECT, 2025
** command.c
** File description:
** Functions to parse and manage commands
*/

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <fcntl.h>

#include "include/command.h"
#include "include/client.h"
#include "logs.h"
#include <errno.h>

void append_client_out_buffer(client_t *client, const char *format, ...)
{
    va_list args;
    char *new_buffer = NULL;
    size_t size = 0;

    va_start(args, format);
    size = vsnprintf(NULL, 0, format, args);
    va_end(args);
    new_buffer = malloc(sizeof(char) * (size + 1));
    if (new_buffer == NULL)
        display_error("Memory allocation failed for client output buffer");
    va_start(args, format);
    vsnprintf(new_buffer, size + 1, format, args);
    va_end(args);
    if (client->out_buffer == NULL)
        client->out_buffer = new_buffer;
    else
        add_to_buffer(&client->out_buffer, new_buffer);
}

static bool get_client_buffer(client_t *client, int fd, zappy_t *zappy)
{
    char *buffer = malloc(sizeof(char) * 1024);
    ssize_t bytes_read = 0;

    if (buffer == NULL)
        display_error("Memory allocation failed for client buffer");
    bytes_read = read(fd, buffer, 1023);
    if (bytes_read <= 0) {
        if (bytes_read == -1)
            perror("Read error");
        free(buffer);
        client->is_connected = false;
        return false;
    }
    buffer[bytes_read] = '\0';
    if (client->in_buffer == NULL)
        client->in_buffer = buffer;
    else
        add_to_buffer(&client->in_buffer, buffer);
    extract_commands(client, zappy);
    return true;
}

void handle_client_command(zappy_t *zappy, int fd)
{
    client_t *current = zappy->clients;
    char buffer[1024];

    while (current != NULL && current->fd != fd) {
        current = current->next;
    }
    if (current == NULL)
        return;
    if (get_client_buffer(current, fd, zappy) == false) {
        if (!current->is_graphic && !current->is_waiting_id) {
            sprintf(buffer, "pdi #%d\n", current->stats.id);
            send_data_to_graphics(zappy, buffer);
        }
        LOG_INFO("Client with fd %d disconnected", fd);
        remove_client(zappy, fd);
    }
}

void send_client_command(zappy_t *zappy, int fd)
{
    client_t *current = zappy->clients;

    while (current != NULL && current->fd != fd)
        current = current->next;
    if (current == NULL || current->out_buffer == NULL)
        return;
    if (fcntl(fd, F_GETFD) == -1 && errno == EBADF)
        return;
    if (write(fd, current->out_buffer, strlen(current->out_buffer)) == -1) {
        perror("Write error");
        LOG_INFO("[%i]: Failed to send command %s",
            current->fd, current->out_buffer);
    } else {
        LOG_INFO("[%i]: Sent %s", current->fd, current->out_buffer);
    }
    free(current->out_buffer);
    current->out_buffer = NULL;
}
