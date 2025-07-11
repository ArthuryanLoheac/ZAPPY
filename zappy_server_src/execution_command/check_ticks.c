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
#include <string.h>

#include "../include/client.h"
#include "command.h"
#include "command_handler.h"

static void exec_command_tick(zappy_t *zappy, client_t *client,
    waitingCommands_t *command)
{
    command_handler_t handler;

    if (!command || command->command == NULL || command->command[0] == NULL)
        return;
    if (client->is_graphic)
        handler = get_gui_command_handler(command->command[0]);
    else
        handler = get_player_command_handler(command->command[0]);
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
    if (!client->is_connected || client->is_waiting_id || client->is_graphic)
        return;
    client->stats.tickLife--;
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

static void check_consume(zappy_t *zappy)
{
    pos_elevation_t *poses = zappy->pos_elevations;
    pos_elevation_t *prev = NULL;
    char buffer[256];

    while (poses) {
        consume_incantation(zappy, poses->x, poses->y, poses->level);
        sprintf(buffer, "pie %d %d Current level: %d\n", poses->x, poses->y,
            poses->level + 1);
        send_data_to_graphics(zappy, buffer);
        prev = poses;
        poses = poses->next;
        free(prev);
        prev = NULL;
    }
    if (prev)
        free(prev);
    zappy->pos_elevations = NULL;
}

static void check_fail(zappy_t *zappy)
{
    pos_elevation_t *poses = zappy->pos_elevationsFail;
    pos_elevation_t *prev = NULL;
    char buffer[256];

    while (poses) {
        sprintf(buffer, "pie %d %d ko\n", poses->x, poses->y);
        send_data_to_graphics(zappy, buffer);
        prev = poses;
        poses = poses->next;
        free(prev);
        prev = NULL;
    }
    if (prev)
        free(prev);
    zappy->pos_elevationsFail = NULL;
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
    check_consume(zappy);
    check_fail(zappy);
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
    while (zappy->durationTickLeft <= 0) {
        reduce_tick_all(zappy);
        zappy->durationTickLeft = zappy->durationTick;
        zappy->tickCount++;
        if (zappy->tickCount == 20) {
            update_map(zappy->map->grid, zappy->parser);
            zappy->tickCount = 0;
        }
    }
}
