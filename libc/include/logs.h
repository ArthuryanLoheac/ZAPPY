/*
** EPITECH PROJECT, 2025
** ZAPPY
** File description:
** Functions used to log stuff & colors attributed to them
*/

#ifndef LOGS_H_
    #define LOGS_H_
    // This is required to stop the c++ linker to mangle the name of
    // these C functions.
    #ifdef __cplusplus
extern "C" {
    #endif

    // Purple
    #define COLOR_DEBUG "\e[0;35m"
    // Cyan
    #define COLOR_INFO "\e[0;36m"
    // Yellow
    #define COLOR_WARNING "\e[0;93m"
    // Red
    #define COLOR_ERROR "\e[1;91m"
    // Bright red
    #define COLOR_FATAL "\e[1;91m"
    // Classic white
    #define NO_COLOR "\e[0;37m"

    typedef enum log_level {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    } log_level_e;

    typedef struct log_level_str_s {
        char str[10];
        char color[10];
    } log_level_str_t;

    void set_minimum_log_level(log_level_e level);

    void my_log(log_level_e level, const char *format, ...);


    #define LOG_DEBUG(format, ...) (my_log(DEBUG, format, ##__VA_ARGS__))

    #define LOG_INFO(format, ...) (my_log(INFO, format, ##__VA_ARGS__))

    #define LOG_WARNING(format, ...) (my_log(WARNING, format, ##__VA_ARGS__))

    #define LOG_ERROR(format, ...) (my_log(ERROR, format, ##__VA_ARGS__))

    #define LOG_FATAL(format, ...) (my_log(FATAL, format, ##__VA_ARGS__))

    #ifdef __cplusplus
}  // extern "C"
    #endif

#endif /* !LOGS_H_ */
