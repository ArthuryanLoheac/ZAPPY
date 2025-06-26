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
    else {
        add_to_buffer(&client->out_buffer, new_buffer);
        free(new_buffer);
    }
}

static bool return_bytes_read(ssize_t bytes_read, char *buffer,
    client_t *client)
{
    if (bytes_read == -1)
        perror("Read error");
    (void) buffer;
    client->is_connected = false;
    return false;
}

static bool get_client_buffer(client_t *client, int fd, zappy_t *zappy)
{
    char *buffer = malloc(sizeof(char) * 1024);
    ssize_t bytes_read = 0;

    if (buffer == NULL)
        display_error("Memory allocation failed for client buffer");
    bytes_read = read(fd, buffer, 1023);
    if (bytes_read <= 0)
        return return_bytes_read(bytes_read, buffer, client);
    buffer[bytes_read] = '\0';
    if (client->in_buffer == NULL)
        client->in_buffer = buffer;
    else {
        add_to_buffer(&client->in_buffer, buffer);
    }
    extract_commands(client, zappy);
    return true;
}

static void create_egg_and_send(zappy_t *zappy, client_t *removed)
{
    char buffer[256];
    egg_t *new_egg = malloc(sizeof(egg_t));

    if (new_egg == NULL)
        display_error("Failed to create egg");
    new_egg->x = rand() % zappy->parser->width;
    new_egg->y = rand() % zappy->parser->height;
    new_egg->id = zappy->map->id_egg;
    new_egg->team_name = strdup(removed->stats.team_name);
    new_egg->next = zappy->map->eggs;
    zappy->map->eggs = new_egg;
    zappy->map->id_egg += 1;
    snprintf(buffer, sizeof(buffer), "enw #%d #-1 %d %d\n",
        new_egg->id, new_egg->x, new_egg->y);
    send_data_to_graphics(zappy, buffer);
}

static void check_add_egg(zappy_t *zappy, client_t *removed)
{
    client_t *current = zappy->clients;
    egg_t *egg = zappy->map->eggs;
    int nb_client_in_team = 0;
    int nb_egg = 0;

    while (current != NULL) {
        if (strcmp(current->stats.team_name, removed->stats.team_name) == 0
            && current->stats.id != removed->stats.id)
            nb_client_in_team++;
        current = current->next;
    }
    while (egg != NULL) {
        if (strcmp(egg->team_name, removed->stats.team_name) == 0)
            nb_egg++;
        egg = egg->next;
    }
    for (int i = nb_egg + nb_client_in_team;
        i < zappy->parser->clients_per_team; i++)
        create_egg_and_send(zappy, removed);
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
            check_add_egg(zappy, current);
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
    current->out_buffer = NULL;
}
