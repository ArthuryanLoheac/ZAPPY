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

#include "include/logs.h"

// Global var used here to save the log level between each log calls
enum log_level_e actual_log_level = ERROR;

static char *get_timestamp(void)
{
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char *timestamp = malloc(20 * sizeof(char));

    strftime(timestamp, 20 * sizeof(char), "%F %T", tm_info);
    return timestamp;
}

static void remove_newlines(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        if (str[i] == '\n')
            str[i] = '#';
}

// This function has 5 arguments to allow more modularity in the logs
// messages.
// If the coding style infraction is that big of an issue, you can get
// rid of the color.
void log_internal(enum log_level_e level, char *level_str, char *color,
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
    remove_newlines(msg);
    printf("%s  [%s%s%s]\t%s\n",
        timestamp, color, level_str, NO_COLOR, msg);
    free(timestamp);
}

void set_log_level(enum log_level_e level)
{
    actual_log_level = level;
}
