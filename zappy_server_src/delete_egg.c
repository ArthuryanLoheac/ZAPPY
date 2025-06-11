/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** process_command
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

static void delete_egg(egg_t *prev, egg_t *current, zappy_t *zappy_ptr)
{
    if (prev == NULL) {
        zappy_ptr->map->eggs = current->next;
    } else {
        prev->next = current->next;
    }
    free(current->team_name);
    free(current);
}

void delete_egg_team_name(zappy_t *zappy_ptr, char *team_name)
{
    egg_t *current = zappy_ptr->map->eggs;
    egg_t *prev = NULL;

    while (current) {
        if (strcmp(current->team_name, team_name) == 0) {
            delete_egg(prev, current, zappy_ptr);
            return;
        }
        prev = current;
        current = current->next;
    }
}
