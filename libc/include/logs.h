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

    enum log_level_e {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };

    void set_log_level(enum log_level_e level);

    // If I reduce the number of arguments, the complexity of the function
    // may increase significally
    void log_internal(enum log_level_e level, const char *level_str,
        const char *color, const char *format, ...);

    // These debug macros functions are too long for the coding style ToT
    // There seems to be now way to legally respect the 80chars rule & no \ at
    // end of file ...

    // I can replace them with functions but this will lead to repeated functions.
    // It is why I use macros here, to not have 5 (almost) same functions and
    // not having to say smth like log(DEBUG, "Hello");
    #define LOG_DEBUG(format, ...) \
    (log_internal(DEBUG, "DEBUG", COLOR_DEBUG, format, ##__VA_ARGS__))

    #define LOG_INFO(format, ...) \
    (log_internal(INFO, "INFO", COLOR_INFO, format, ##__VA_ARGS__))

    #define LOG_WARNING(format, ...) \
    (log_internal(WARNING, "WARNING", COLOR_WARNING, format, ##__VA_ARGS__))

    #define LOG_ERROR(format, ...) \
    (log_internal(ERROR, "ERROR", COLOR_ERROR, format, ##__VA_ARGS__))

    #define LOG_FATAL(format, ...) \
    (log_internal(FATAL, "FATAL", COLOR_FATAL, format, ##__VA_ARGS__))

    #ifdef __cplusplus
}  // extern "C"
    #endif

#endif /* !LOGS_H_ */
