/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Functions used to log stuff & colors attributed to them
*/

#ifndef LOGS_H_
    #define LOGS_H_

#define COLOR_DEBUG "\e[0;35m"  // Purple
#define COLOR_INFO "\e[0;36m"   // Cyan
#define COLOR_WARNING "\e[0;93m"// Yellow
#define COLOR_ERROR "\e[1;91m"  // Red
#define COLOR_FATAL "\e[1;91m"  // Bright red
#define NO_COLOR "\e[0;37m" // Classic white

typedef enum log_level {
    Debug,
    Info,
    Warning,
    Error,
    Fatal
} log_level_e;

void set_log_level(log_level_e level);

void log_internal(log_level_e level, char *level_str, char *color,
     const char *format, ...);

#define LOG_DEBUG(format, ...) \
    (log_internal(Debug, "DEBUG", COLOR_DEBUG, format, ##__VA_ARGS__))

#define LOG_INFO(format, ...) \
    (log_internal(Info, "INFO", COLOR_INFO, format, ##__VA_ARGS__))

#define LOG_WARNING(format, ...) \
    (log_internal(Warning, "WARNING", COLOR_WARNING, format, ##__VA_ARGS__))

#define LOG_ERROR(format, ...) \
    (log_internal(Error, "ERROR", COLOR_ERROR, format, ##__VA_ARGS__))

#define LOG_FATAL(format, ...) \
    (log_internal(Fatal, "FATAL", COLOR_FATAL, format, ##__VA_ARGS__))

#endif /* !LOGS_H_ */
