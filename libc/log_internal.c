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

static const log_level_str_t LOG_LEVEL_STRS_LIST[] = {
    {"DEBUG", COLOR_DEBUG},
    {"INFO", COLOR_INFO},
    {"WARNING", COLOR_WARNING},
    {"ERROR", COLOR_ERROR},
    {"FATAL", COLOR_FATAL}
};

static const unsigned int LOGS_LEVELS_COUNT = 5;

static log_level_e *get_minimum_log_level_ptr(void)
{
    static log_level_e level = ERROR;

    return &level;
}

void set_minimum_log_level(log_level_e level)
{
    *get_minimum_log_level_ptr() = level;
}

static log_level_e get_minimum_log_level(void)
{
    return *get_minimum_log_level_ptr();
}

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
            str[i] = '|';
}

void my_log(log_level_e level, const char *format, ...)
{
    char *timestamp;
    char msg[1024];
    va_list args;

    if (level < get_minimum_log_level() || level > LOGS_LEVELS_COUNT)
        return;
    timestamp = get_timestamp();
    va_start(args, format);
    vsnprintf(msg, sizeof(msg), format, args);
    va_end(args);
    remove_newlines(msg);
    printf("%s  [%s%s%s]\t%s\n",
        timestamp, LOG_LEVEL_STRS_LIST[level].color,
        LOG_LEVEL_STRS_LIST[level].str, NO_COLOR, msg);
    free(timestamp);
}
