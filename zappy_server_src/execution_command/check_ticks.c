/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** check_ticks
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#include "../include/client.h"

static void update_pos_player(zappy_t *zappy, client_t *client)
{
    char buffer[256];

    sprintf(buffer, "ppo #%d %d %d %d\n", client->id, client->x, client->y,
        client->orientation);
    send_data_to_graphics(zappy, buffer);
}

static void forward_command(zappy_t *zappy, client_t *client)
{
    if (client->orientation == 3)
        client->y = (client->y + 1) % zappy->parser->height;
    if (client->orientation == 2)
        client->x += (client->x + 1) % zappy->parser->width;
    if (client->orientation == 1) {
        client->y -= 1;
        if (client->y < 0)
            client->y = zappy->parser->height - 1;
    }
    if (client->orientation == 4) {
        client->x -= 1;
        if (client->x < 0)
            client->x = zappy->parser->width - 1;
    }
    update_pos_player(zappy, client);
    add_to_buffer(&client->out_buffer, "ok\n");
}

static void rotate_command(zappy_t *zappy, client_t *client, int i)
{
    client->orientation += i;
    update_pos_player(zappy, client);
    add_to_buffer(&client->out_buffer, "ok\n");
}

static void exec_command_tick(zappy_t *zappy, client_t *client,
    waitingCommands_t *command)
{
    (void) zappy;
    (void) client;
    if (!command || command->command == NULL || command->command[0] == NULL)
        return;
    if (strcmp(command->command[0], "FORWARD") == 0) {
        forward_command(zappy, client);
    } else if (strcmp(command->command[0], "RIGHT") == 0) {
        rotate_command(zappy, client, 1);
    } else if (strcmp(command->command[0], "LEFT") == 0) {
        rotate_command(zappy, client, -1);
    } else {
        add_to_buffer(&client->out_buffer, "ko\n");
    }
}

static void reduce_tick_all(zappy_t *zappy)
{
    client_t *client = zappy->clients;
    waitingCommands_t *command;

    while (client) {
        if (client->waiting_commands == NULL) {
            client = client->next;
            continue;
        }
        client->waiting_commands->ticksLeft--;
        if (client->waiting_commands->ticksLeft <= 0) {
            exec_command_tick(zappy, client, client->waiting_commands);
            command = client->waiting_commands;
            client->waiting_commands = client->waiting_commands->next;
            free(command);
        }
        client = client->next;
    }
}

static float get_delta_time(zappy_t *zappy)
{
    struct timeval current_time;
    float delta = 0;

    gettimeofday(&current_time, NULL);
    if (zappy->last_time.tv_sec == 0 && zappy->last_time.tv_usec == 0)
        zappy->last_time = current_time;
    delta = (current_time.tv_sec - zappy->last_time.tv_sec) +
            (current_time.tv_usec - zappy->last_time.tv_usec) / 1000000.0f;
    zappy->last_time = current_time;
    return delta;
}

void check_ticks(zappy_t *zappy)
{
    float delta = get_delta_time(zappy);

    zappy->durationTickLeft -= delta;
    if (zappy->durationTickLeft <= 0) {
        reduce_tick_all(zappy);
        zappy->durationTickLeft = zappy->durationTick;
    }
}
