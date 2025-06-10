/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** logs
*/

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <stdlib.h>

#include "logs.h"

log_level_e actual_log_level = Info;

static char *get_timestamp(void)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char *timestamp = malloc(20 * sizeof(char));

    strftime(timestamp, 20 * sizeof(char), "%F %T", tm_info);
    return timestamp;
}

void log_internal(log_level_e level, char *level_str, char *color,
    const char *format, ...)
{
    char *timestamp;
    char msg[1024];
    va_list args;

    if (level < actual_log_level)
        return;
    timestamp = get_timestamp();
    va_start(args, format);
    vsnprintf(msg, sizeof(msg), format, args);
    va_end(args);
    printf("%s  [%s%s%s]\t%s\n",
        timestamp, color, level_str, NO_COLOR, msg);
    free(timestamp);
}

void set_log_level(log_level_e level)
{
    actual_log_level = level;
}
