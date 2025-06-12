/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** check_ticks
*/

#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>

#include "../include/client.h"
#include "command.h"
#include "command_handler.h"

static void exec_command_tick(zappy_t *zappy, client_t *client,
    waitingCommands_t *command)
{
    command_handler_t handler;

    if (!command || command->command == NULL || command->command[0] == NULL)
        return;
    handler = get_command_handler(command->command[0]);
    if (handler == NULL)
        add_to_buffer(&client->out_buffer, "ko\n");
    else
        handler(zappy, client, &command->command[1]);
}

static void send_pdi_graphic(client_t *c, zappy_t *zappy)
{
    char pdi_data[256];

    snprintf(pdi_data, sizeof(pdi_data),
        "pdi #%d\n", c->stats.id);
    send_data_to_graphics(zappy, pdi_data);
}

static void send_pin_graphic(client_t *c, zappy_t *zappy)
{
    char pin_data[256];
    stats_t s = c->stats;

    snprintf(pin_data, sizeof(pin_data),
        "pin #%d %d %d %d %d %d %d %d %d %d\n",
        s.id, s.x, s.y, s.inventory.food, s.inventory.linemate,
        s.inventory.deraumere, s.inventory.sibur, s.inventory.mendiane,
        s.inventory.phiras, s.inventory.thystame);
    send_data_to_graphics(zappy, pin_data);
}

static void handle_life_tick(client_t *client, zappy_t *zappy)
{
    if (!client->is_connected || client->is_waiting_id)
        return;
    client->stats.tickLife--;
    printf("Client %d tickLife: %d\n", client->stats.id, client->stats.tickLife);
    if (client->stats.tickLife <= 0) {
        if (client->stats.inventory.food > 0) {
            client->stats.inventory.food--;
            client->stats.tickLife = 126;
            send_pin_graphic(client, zappy);
        } else {
            add_to_buffer(&client->out_buffer, "dead\n");
            client->is_connected = false;
            client->is_waiting_id = false;
            send_pdi_graphic(client, zappy);
            client->stats.id = -1;
        }
    }
}

static void reduce_tick_all(zappy_t *zappy)
{
    client_t *client = zappy->clients;
    waitingCommands_t *command;

    while (client) {
        handle_life_tick(client, zappy);
        if (client->waiting_commands == NULL || !client->is_connected) {
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
        zappy->durationTickLeft += zappy->durationTick;
        check_ticks(zappy);
    }
}
